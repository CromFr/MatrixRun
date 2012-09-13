#include "StdInput.hpp"

using namespace std;
using namespace boost;
using namespace irr;


namespace mrio
{


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
		StdInput::StdInput() :
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


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
		bool StdInput::OnEvent(const SEvent& event)
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


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
		StdInput::State StdInput::GetMouseButtonState(MouseButton button)const
		{
			switch(button)
			{
				case left:		return m_stateLeft;
				case right:		return m_stateRight;
				case middle:	return m_stateMiddle;
				default: 		return unknown;
			}
		}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
		void StdInput::SetWheelCallback(boost::function<void(float)> func)
		{
			m_bUseWheelCallback = true;
			m_WheelCallback = func;
		}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
		void StdInput::AddMouseEvent(core::vector2df pos, MouseButton button, State state)
		{
			MouseEvent e;
			e.pos = pos;
			e.button = button;
			e.state = state;
			m_qMEvents.push(e);
		}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
		void StdInput::AddKeyboardEvent(EKEY_CODE key, State state)
		{
			KeyboardEvent e;
			e.key = key;
			e.state = state;
			m_qKBEvents.push(e);
		}
}
