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

	class Mouse : public IEventReceiver
	{
	public:
			enum MouseButton
			{
				left, right, middle, wheel
			};
			enum MouseState
			{
				down, up, unknown
			};
			struct MouseEvent
			{
				core::vector2df pos;
				MouseButton button;
				MouseState state;
			};



			Mouse() :
				m_vMousePos(0,0)
			{
				m_stateLeft=up;
				m_stateRight=up;
				m_stateMiddle=up;

				m_bUseWheelCallback=false;
			}


			//Override
			virtual bool OnEvent(const SEvent& event)
			{


				if (event.EventType == EET_MOUSE_INPUT_EVENT)
				{
						switch(event.MouseInput.Event)
						{
						case EMIE_LMOUSE_PRESSED_DOWN:
							m_stateLeft = down;
							AddEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), left, down);
							break;
						case EMIE_RMOUSE_PRESSED_DOWN:
							m_stateRight = down;
							AddEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), right, down);
							break;
						case EMIE_MMOUSE_PRESSED_DOWN:
							m_stateMiddle = down;
							AddEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), middle, down);
							break;
						case EMIE_LMOUSE_LEFT_UP:
							m_stateLeft = up;
							AddEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), left, up);
							break;
						case EMIE_RMOUSE_LEFT_UP:
							m_stateRight = up;
							AddEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), right, up);
							break;
						case EMIE_MMOUSE_LEFT_UP:
							m_stateMiddle = up;
							AddEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), middle, up);
							break;

						case EMIE_MOUSE_WHEEL:
							if(event.MouseInput.Wheel>=1) 		AddEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), wheel, up);
							else if(event.MouseInput.Wheel<=-1)	AddEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), wheel, down);
							else								AddEvent(core::vector2df(event.MouseInput.X, event.MouseInput.Y), wheel, unknown);

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

				return false;
			}

			const core::vector2di& GetMousePosition(void) const
			{
					return m_vMousePos;
			}

			bool GetIsMouseEvent()const {return m_qEvents.size()>0;}
			const struct MouseEvent* GetMouseLastEvent()const {return &m_qEvents.front();}
			void DeleteLastEvent(){m_qEvents.pop();}


			MouseState GetMouseButtonState(MouseButton button)const
			{
				switch(button)
				{
					case left:		return m_stateLeft;
					case right:		return m_stateRight;
					case middle:	return m_stateMiddle;
					default: 		return unknown;
				}
			}


			void SetWheelCallback(boost::function<void(float)> func)
			{
				m_bUseWheelCallback = true;
				m_WheelCallback = func;
			}

	private:
			void AddEvent(core::vector2df pos, MouseButton button, MouseState state)
			{
				MouseEvent e;
				e.pos = pos;
				e.button = button;
				e.state = state;
				m_qEvents.push(e);
			}

			core::vector2di m_vMousePos;
			MouseState m_stateLeft, m_stateRight, m_stateMiddle;

			bool m_bUseWheelCallback;
			boost::function<void(float)> m_WheelCallback;


			queue<MouseEvent> m_qEvents;


	};
}

#endif // MOUSE_HPP_INCLUDED
