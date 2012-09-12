#ifndef MOUSE_HPP_INCLUDED
#define MOUSE_HPP_INCLUDED

#include <queue>
using namespace std;

#include <irrlicht.h>
using namespace irr;

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace mrio
{

	class StdInput : public IEventReceiver
	{
	public:
			enum MouseButton
			{
				left, right, middle, wheel
			};
			enum State
			{
				down, up, unknown
			};
			struct MouseEvent
			{
				core::vector2df pos;
				MouseButton button;
				State state;
			};

			struct KeyboardEvent
			{
				EKEY_CODE key;
				State state;

			};



			StdInput() :
				m_vMousePos(0,0)
			{
				m_stateLeft=up;
				m_stateRight=up;
				m_stateMiddle=up;

				m_bUseWheelCallback=false;

				for(int i=0 ; i<KEY_KEY_CODES_COUNT ; i++)
					m_aKeyState[i]=up;

				m_bCaptureKeyboard=false;
				m_bCaptureMouse=false;
			}

			void SetMouseCapture(bool b){m_bCaptureMouse=b;}
			void SetKeyboardCapture(bool b){m_bCaptureKeyboard=b;}


			//Override
			virtual bool OnEvent(const SEvent& event)
			{
				if (event.EventType == EET_MOUSE_INPUT_EVENT && m_bCaptureMouse)
				{
						switch(event.MouseInput.Event)
						{
						case EMIE_LMOUSE_PRESSED_DOWN:
							m_stateLeft = down;
							AddMouseEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), left, down);
							break;
						case EMIE_RMOUSE_PRESSED_DOWN:
							m_stateRight = down;
							AddMouseEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), right, down);
							break;
						case EMIE_MMOUSE_PRESSED_DOWN:
							m_stateMiddle = down;
							AddMouseEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), middle, down);
							break;
						case EMIE_LMOUSE_LEFT_UP:
							m_stateLeft = up;
							AddMouseEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), left, up);
							break;
						case EMIE_RMOUSE_LEFT_UP:
							m_stateRight = up;
							AddMouseEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), right, up);
							break;
						case EMIE_MMOUSE_LEFT_UP:
							m_stateMiddle = up;
							AddMouseEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), middle, up);
							break;

						case EMIE_MOUSE_WHEEL:
							if(event.MouseInput.Wheel>=1) 		AddMouseEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), wheel, up);
							else if(event.MouseInput.Wheel<=-1)	AddMouseEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), wheel, down);
							else								AddMouseEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), wheel, unknown);

							if(m_bUseWheelCallback)
								m_WheelCallback((float)event.MouseInput.Wheel);
							break;

						case EMIE_MOUSE_MOVED:
							m_vMousePos.X = event.MouseInput.X;
							m_vMousePos.Y = event.MouseInput.Y;
							break;

						default:
							break;
						}
				}
				else if (event.EventType == EET_KEY_INPUT_EVENT && m_bCaptureKeyboard)
				{
					State s;
					if(event.KeyInput.PressedDown)	s=down;
					else							s=up;

					AddKeyboardEvent(event.KeyInput.Key, s);

					m_aKeyState[event.KeyInput.Key-1] = s;
				}

				return false;
			}

			const core::vector2di& GetMousePosition(void) const
			{
				return m_vMousePos;
			}

			bool GetIsMouseEvent()const {return m_qMEvents.size()>0;}
			const struct MouseEvent* GetLastMouseEvent()const {return &m_qMEvents.front();}
			void DropLastMouseEvent(){m_qMEvents.pop();}


			bool GetIsKeyboardEvent()const {return m_qKBEvents.size()>0;}
			const struct KeyboardEvent* GetLastKeyboardEvent()const {return &m_qKBEvents.front();}
			void DropLastKeyboardEvent(){m_qKBEvents.pop();}


			State GetMouseButtonState(MouseButton button)const
			{
				switch(button)
				{
					case left:		return m_stateLeft;
					case right:		return m_stateRight;
					case middle:	return m_stateMiddle;
					default: 		return unknown;
				}
			}


			State GetKeyState(EKEY_CODE key)const
			{
				return m_aKeyState[key-1];
			}

			void SetWheelCallback(boost::function<void(float)> func)
			{
				m_bUseWheelCallback = true;
				m_WheelCallback = func;
			}

	private:
			void AddMouseEvent(core::vector2df pos, MouseButton button, State state)
			{
				MouseEvent e;
				e.pos = pos;
				e.button = button;
				e.state = state;
				m_qMEvents.push(e);
			}
			void AddKeyboardEvent(EKEY_CODE key, State state)
			{
				KeyboardEvent e;
				e.key = key;
				e.state = state;
				m_qKBEvents.push(e);
			}

			bool m_bCaptureKeyboard;
			bool m_bCaptureMouse;

			core::vector2di m_vMousePos;
			State m_stateLeft, m_stateRight, m_stateMiddle;

			bool m_bUseWheelCallback;
			boost::function<void(float)> m_WheelCallback;

			State m_aKeyState[KEY_KEY_CODES_COUNT];


			queue<MouseEvent> m_qMEvents;
			queue<KeyboardEvent> m_qKBEvents;


	};
}

#endif // MOUSE_HPP_INCLUDED
