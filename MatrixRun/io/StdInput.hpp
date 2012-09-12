#ifndef STDINPUT_HPP_INCLUDED
#define STDINPUT_HPP_INCLUDED

#include <queue>

#include <irrlicht.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace mrio
{

	class StdInput : public irr::IEventReceiver
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
				irr::core::vector2df pos;
				MouseButton button;
				State state;
			};

			struct KeyboardEvent
			{
				irr::EKEY_CODE key;
				State state;

			};

			StdInput();

			void SetMouseCapture(bool b){m_bCaptureMouse=b;}
			void SetKeyboardCapture(bool b){m_bCaptureKeyboard=b;}


			//Override
			virtual bool OnEvent(const irr::SEvent& event);

			const irr::core::vector2di& GetMousePosition(void) const	{return m_vMousePos;}

			bool GetIsMouseEvent()const {return m_qMEvents.size()>0;}
			const struct MouseEvent* GetLastMouseEvent()const {return &m_qMEvents.front();}
			void DropLastMouseEvent(){m_qMEvents.pop();}


			bool GetIsKeyboardEvent()const {return m_qKBEvents.size()>0;}
			const struct KeyboardEvent* GetLastKeyboardEvent()const {return &m_qKBEvents.front();}
			void DropLastKeyboardEvent(){m_qKBEvents.pop();}


			State GetMouseButtonState(MouseButton button)const;


			State GetKeyState(irr::EKEY_CODE key)const {return m_aKeyState[key-1];}

			void SetWheelCallback(boost::function<void(float)> func);

	private:
			void AddMouseEvent(irr::core::vector2df pos, MouseButton button, State state);
			void AddKeyboardEvent(irr::EKEY_CODE key, State state);

			bool m_bCaptureKeyboard;
			bool m_bCaptureMouse;

			irr::core::vector2di m_vMousePos;
			State m_stateLeft, m_stateRight, m_stateMiddle;

			bool m_bUseWheelCallback;
			boost::function<void(float)> m_WheelCallback;

			State m_aKeyState[irr::KEY_KEY_CODES_COUNT];


			std::queue<MouseEvent> m_qMEvents;
			std::queue<KeyboardEvent> m_qKBEvents;


	};
}

#endif // STDINPUT_HPP_INCLUDED
