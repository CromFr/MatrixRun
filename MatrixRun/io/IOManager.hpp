#ifndef IOMANAGER_HPP_INCLUDED
#define IOMANAGER_HPP_INCLUDED

#include <iostream>
#include <queue>
using namespace std;

#include <irrlicht.h>
using namespace irr;

#include "WiimoteHandler.hpp"
#include "StdInput.hpp"

#include "../io/transition.hpp"

namespace mrio
{

	class IOManager
	{
	public:
		enum InputDevice
			{ mouse, keyboard, wiimote };

		enum Cursor
			{ right, left };

		enum Button
			{ primary, secondary, menu };

		enum Event
			{ pressed, released };

		struct IOCursorEvent
		{
			Cursor cursor;
			core::vector2di pos;
			Button button;
			Event event;
		};



		IOManager(InputDevice CameraControl, InputDevice CursorControl, const core::vector2di& vScreenDim):
			m_vScreenDim(vScreenDim)
		{
			m_StdIn = new StdInput();
			m_WM = 0;

			m_CamCtrl=CameraControl;
			m_CurCtrl=CursorControl;

			switch(m_CamCtrl)
			{
			case mouse:
				fMouseCtrlCamZ = -100;
				m_StdIn->SetMouseCapture(true);
				break;

			case keyboard:
				m_StdIn->SetKeyboardCapture(true);
				break;

			case wiimote:
				m_WM = new WiimoteHandler();
				break;
			}

			switch(m_CurCtrl)
			{
			case mouse:
				m_CursorCtrlSelectedCur = left;
				m_StdIn->SetWheelCallback(boost::bind(&IOManager::SwitchCursor, this, _1));
				m_StdIn->SetMouseCapture(true);
				break;

			case keyboard:
				m_StdIn->SetKeyboardCapture(true);
				break;

			case wiimote:
				if(m_WM == 0) m_WM = new WiimoteHandler();
				break;
			}


		}

		~IOManager()
		{
			delete m_StdIn;
			delete m_WM;
		}


		/*===========================================================================
		\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
		//////////////////////////////////////////////////////////////////////////////
		===========================================================================*/
		const core::vector3df& GetCameraPosition(bool* outEnoughIRSrc=0)
		{
			if(m_CamCtrl==mouse)
			{
				core::vector2di pos(m_StdIn->GetMousePosition());
				pos.X = 300*pos.X/m_vScreenDim.X-150;
				pos.Y = 300*pos.Y/m_vScreenDim.Y-150;

				if(outEnoughIRSrc>0)
					*outEnoughIRSrc = true;

				m_vLastCameraPos.set(pos.X, pos.Y, fMouseCtrlCamZ);
			}
			else if(m_CamCtrl==keyboard)
			{
				float SPEED(5.0);
				if(m_StdIn->GetKeyState(KEY_UP) == StdInput::down)
				{
					if(m_StdIn->GetKeyState(KEY_RSHIFT) == StdInput::down)
						m_vLastCameraPos.Z += SPEED;
					else
						m_vLastCameraPos.Y += SPEED;
				}
				if(m_StdIn->GetKeyState(KEY_DOWN) == StdInput::down)
				{
					if(m_StdIn->GetKeyState(KEY_RSHIFT) == StdInput::down)
						m_vLastCameraPos.Z -= SPEED;
					else
						m_vLastCameraPos.Y -= SPEED;
				}
				if(m_StdIn->GetKeyState(KEY_LEFT) == StdInput::down)
				{
					m_vLastCameraPos.X -= SPEED;
				}
				if(m_StdIn->GetKeyState(KEY_RIGHT) == StdInput::down)
				{
					m_vLastCameraPos.X += SPEED;
				}

				if(outEnoughIRSrc>0)
					*outEnoughIRSrc = true;
			}
			else if(m_CamCtrl==wiimote)
			{
				try
				{
					m_vLastCameraPos.set(Wiimote3dToWorld3d(m_WM->GetPlayerPos()));

					if(outEnoughIRSrc>0)
						*outEnoughIRSrc = true;
				}
				catch(int e)
				{
					if(e == EXC_WIIPOS_NOT_ENOUGH_IRSRC && outEnoughIRSrc>0)
						*outEnoughIRSrc = false;
				}

			}
			else
				cerr<<"/!\\  Unknown camera controller !"<<endl;

			return m_vLastCameraPos;
		}
		/*===========================================================================
		\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
		//////////////////////////////////////////////////////////////////////////////
		===========================================================================*/
		core::vector2di GetCursorPosition(Cursor cur, bool* outEnoughIRSrc=0)
		{
			if(m_CurCtrl==mouse)
			{
				//update cursor pos
				if(m_CursorCtrlSelectedCur == left)
				{
					m_vLastLeftCursorPos.set(m_StdIn->GetMousePosition());
				}
				else
				{
					m_vLastRightCursorPos.set(m_StdIn->GetMousePosition());
				}

				if(cur==left)
					return m_vLastLeftCursorPos;
				else
					return m_vLastRightCursorPos;

				if(outEnoughIRSrc>0)
					*outEnoughIRSrc = true;
			}
			else if(m_CurCtrl==keyboard)
			{
				float SPEED(3.0);
				if(m_StdIn->GetKeyState(KEY_KEY_Z) == StdInput::down)
				{
					if(m_StdIn->GetKeyState(KEY_LSHIFT) == StdInput::down)
						m_vLastRightCursorPos.Y -= SPEED;
					else
						m_vLastLeftCursorPos.Y -= SPEED;
				}
				if(m_StdIn->GetKeyState(KEY_KEY_S) == StdInput::down)
				{
					if(m_StdIn->GetKeyState(KEY_LSHIFT) == StdInput::down)
						m_vLastRightCursorPos.Y += SPEED;
					else
						m_vLastLeftCursorPos.Y += SPEED;
				}
				if(m_StdIn->GetKeyState(KEY_KEY_Q) == StdInput::down)
				{
					if(m_StdIn->GetKeyState(KEY_LSHIFT) == StdInput::down)
						m_vLastRightCursorPos.X -= SPEED;
					else
						m_vLastLeftCursorPos.X -= SPEED;
				}
				if(m_StdIn->GetKeyState(KEY_KEY_D) == StdInput::down)
				{
					if(m_StdIn->GetKeyState(KEY_LSHIFT) == StdInput::down)
						m_vLastRightCursorPos.X += SPEED;
					else
						m_vLastLeftCursorPos.X += SPEED;
				}

				if(outEnoughIRSrc>0)
					*outEnoughIRSrc = true;

				if(cur==left)
					return m_vLastLeftCursorPos;
				else
					return m_vLastRightCursorPos;
			}
			else if(m_CurCtrl==wiimote)
			{
				try
				{
					Wiimote2dPercent pos;
					if(cur==left)
					{
						pos = m_WM->GetCursorPos(0);
						m_vLastLeftCursorPos.set(pos.x*m_vScreenDim.X, pos.y*m_vScreenDim.Y);
					}
					else
					{
						pos = m_WM->GetCursorPos(1);
						m_vLastRightCursorPos.set(pos.x*m_vScreenDim.X, pos.y*m_vScreenDim.Y);
					}

					if(outEnoughIRSrc>0)
						*outEnoughIRSrc = true;
				}
				catch(int e)
				{
					if(e == EXC_WIICUR_NO_IRSRC && outEnoughIRSrc>0)
						*outEnoughIRSrc = false;
				}

				return m_vLastRightCursorPos;
			}
			else
				cerr<<"/!\\  Unknown cursor controller !"<<endl;

			return core::vector2di(0,0);
		}

		/*===========================================================================
		\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
		//////////////////////////////////////////////////////////////////////////////
		===========================================================================*/
		bool GetIsCursorEvent(Cursor cur)
		{
			if(m_CurCtrl==mouse)
			{
				for(  ; m_StdIn->GetIsMouseEvent() ; m_StdIn->DropLastMouseEvent())
				{
					const struct StdInput::MouseEvent* me = m_StdIn->GetLastMouseEvent();
					if(me->button == StdInput::left || me->button == StdInput::right)//<============== Allowed mouse Buttons
						return true;
				}
			}
			else if(m_CurCtrl==keyboard)
			{
				for(  ; m_StdIn->GetIsKeyboardEvent() ; m_StdIn->DropLastKeyboardEvent())
				{
					const struct StdInput::KeyboardEvent* kbe = m_StdIn->GetLastKeyboardEvent();
					if(kbe->key == KEY_KEY_A || kbe->key == KEY_KEY_E)				//<============== Allowed keyboard keys
						return true;
				}
			}
			else if(m_CurCtrl==wiimote)
			{
				int nWM;
				if(cur == left)	nWM = WMHDL_LEFT;
				else			nWM = WMHDL_RIGHT;

				for(  ; m_WM->GetIsButtonEvent(nWM) ; m_WM->DropLastButtonEvent(nWM))
				{
					struct WiimoteCursorEvent wme = m_WM->GetLastButtonEvent(nWM);
					if(wme.button & (WIIMOTE_BUTTON_A|WIIMOTE_BUTTON_B|WIIMOTE_BUTTON_HOME))//<============== Allowed WM Buttons
						return true;
				}
			}
			return false;
		}

		/*===========================================================================
		\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
		//////////////////////////////////////////////////////////////////////////////
		===========================================================================*/
		const IOCursorEvent& GetLastCursorEvent(Cursor cur)
		{
			if(m_CurCtrl==mouse)
			{
				for(  ; m_StdIn->GetIsMouseEvent() ; m_StdIn->DropLastMouseEvent())
				{
					const struct StdInput::MouseEvent* me = m_StdIn->GetLastMouseEvent();
					if(me->button == StdInput::left || me->button == StdInput::right)//<============== Allowed mouse Buttons
					{
						m_LastCursorEvent.cursor = m_CursorCtrlSelectedCur;
						m_LastCursorEvent.pos.set(me->pos.X, me->pos.Y);

						if(me->button == StdInput::left)
							m_LastCursorEvent.button = primary;
						else
							m_LastCursorEvent.button = secondary;

						if(me->state == StdInput::down)
							m_LastCursorEvent.event = pressed;
						else
							m_LastCursorEvent.event = released;

						m_StdIn->DropLastMouseEvent();
						break;
					}
				}
			}
			else if(m_CurCtrl==keyboard)
			{
				for(  ; m_StdIn->GetIsKeyboardEvent() ; m_StdIn->DropLastKeyboardEvent())
				{
					const struct StdInput::KeyboardEvent* kbe = m_StdIn->GetLastKeyboardEvent();
					if(kbe->key == KEY_KEY_A || kbe->key == KEY_KEY_E)
					{
						m_LastCursorEvent.button = primary;

						if(kbe->key == KEY_KEY_A)
						{
							m_LastCursorEvent.cursor = right;
							m_LastCursorEvent.pos.set(m_vLastRightCursorPos);
						}
						else
						{
							m_LastCursorEvent.cursor = left;
							m_LastCursorEvent.pos.set(m_vLastLeftCursorPos);
						}

						if(kbe->state == StdInput::down)
							m_LastCursorEvent.event = pressed;
						else
							m_LastCursorEvent.event = released;

						m_StdIn->DropLastKeyboardEvent();
						break;
					}
				}
			}
			else if(m_CurCtrl==wiimote)
			{
				int nWM;
				if(cur == left)	nWM = WMHDL_LEFT;
				else			nWM = WMHDL_RIGHT;

				for(  ; m_WM->GetIsButtonEvent(nWM) ; m_WM->DropLastButtonEvent(nWM))
				{
					const struct WiimoteCursorEvent wme = m_WM->GetLastButtonEvent(nWM);
					if(wme.button & (WIIMOTE_BUTTON_A|WIIMOTE_BUTTON_B|WIIMOTE_BUTTON_HOME))//<============== Allowed WM Buttons
					{
						m_LastCursorEvent.cursor = cur;
						m_LastCursorEvent.pos.X = wme.pos.x * m_vScreenDim.X;
						m_LastCursorEvent.pos.Y = wme.pos.y * m_vScreenDim.Y;

						if(wme.event == EVENT_PRESSED)	m_LastCursorEvent.event = pressed;
						else							m_LastCursorEvent.event = released;


						if(wme.button & WIIMOTE_BUTTON_B)			m_LastCursorEvent.button = primary;
						else if(wme.button & WIIMOTE_BUTTON_A)		m_LastCursorEvent.button = secondary;
						else if(wme.button & WIIMOTE_BUTTON_HOME)		m_LastCursorEvent.button = menu;

						m_WM->DropLastButtonEvent(nWM);
						break;
					}
				}
			}
			else
				cerr<<"/!\\  Unknown cursor controller !"<<endl;

			return m_LastCursorEvent;
		}

		IEventReceiver* GetEventReceiver(){return m_StdIn;}


		void SwitchCursor(float fWheelDir)
		{
			if(fWheelDir>0)	m_CursorCtrlSelectedCur = left;
			else			m_CursorCtrlSelectedCur = right;
		}



	private:
		InputDevice m_CamCtrl;
		InputDevice m_CurCtrl;
		core::vector2di m_vScreenDim;

		IOCursorEvent m_LastCursorEvent;
		core::vector3df m_vLastCameraPos;
		core::vector2di m_vLastLeftCursorPos;
		core::vector2di m_vLastRightCursorPos;

		StdInput* m_StdIn;
		WiimoteHandler* m_WM;

		queue<IOCursorEvent> m_qCursorEvents;


		//Mouse camera control
		float fMouseCtrlCamZ;

		//Mouse cursor control
		Cursor m_CursorCtrlSelectedCur;






	};
}

#endif // IOMANAGER_HPP_INCLUDED
