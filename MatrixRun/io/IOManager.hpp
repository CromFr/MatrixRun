#ifndef IOMANAGER_HPP_INCLUDED
#define IOMANAGER_HPP_INCLUDED

#include <iostream>
#include <queue>

#include <irrlicht.h>

#include "WiimoteHandler.hpp"
#include "StdInput.hpp"

#include "../io/transition.hpp"

namespace mrio
{
	/**
	@brief Overlay to manage compatibility between all available IO devices
	**/
	class IOManager
	{
	public:
		/**
		@brief List of input devices
		**/
		enum InputDevice
			{ mouse, keyboard, wiimote };

		/**
		@brief Cursor type (left/right)
		**/
		enum Cursor
			{ right, left };

		/**
		@brief Pressable buttons
		**/
		enum Button
			{ primary, secondary, menu };

		/**
		@brief Possible events (pressed/released)
		**/
		enum Event
			{ pressed, released };

		/**
		@brief An event of the cursor
		**/
		struct IOCursorEvent
		{
			Cursor cursor;
			irr::core::vector2di pos;
			Button button;
			Event event;
		};


		/**
		@param CameraControl The device that will control the camera movements
		@param CursorControl The device that will control the two cursors
		@param vScreenDim The dimensions of the screen in pixels
		**/
		IOManager(InputDevice CameraControl, InputDevice CursorControl, const irr::core::vector2di& vScreenDim);

		~IOManager();


		/**
		@brief Gets the camera position in the world
		@param outEnoughIRSrc output param that changes the var to true/false if there is enough/not IR Src to calculate position
		@return The position in irrlicht coordinates
		**/
		const irr::core::vector3df& GetCameraPosition(bool* outEnoughIRSrc=0);

		/**
		@brief Gets the cursor position on screen
		@see GetCameraPosition
		@param cur The cursor to get the position
		**/
		irr::core::vector2di GetCursorPosition(Cursor cur, bool* outEnoughIRSrc=0);

		/**
		@example "Processing events with the IOManager"
		@code
		mrio::IOManager IOMgr(...);
		//(...)
		while( IOMgr.GetIsCursorEvent() )
		{
			const struct mrio::IOManager::IOCursorEvent e = IOMgr.GetLastCursorEvent();

			//Treating the event...
		}
		@endcode
		**/

		/**
		@brief Get if there is an event that has not been treated
		@return true if there is an event
		**/
		bool GetIsCursorEvent();

		/**
		@brief Returns the last cursor event.
		@note Before returning an event, it destroys the last returned event
		**/
		const IOCursorEvent& GetLastCursorEvent();

		/**
		@brief Returns the StdInput instance.
		@note Useful only when initializing the Irrlicht device
		**/
		irr::IEventReceiver* GetEventReceiver(){return m_StdIn;}


	private:
		void SwitchCursor(float fWheelDir);


		InputDevice m_CamCtrl;
		InputDevice m_CurCtrl;
		irr::core::vector2di m_vScreenDim;

		IOCursorEvent m_LastCursorEvent;
		irr::core::vector3df m_vLastCameraPos;
		irr::core::vector2di m_vLastLeftCursorPos;
		irr::core::vector2di m_vLastRightCursorPos;

		StdInput* m_StdIn;
		wm::WiimoteHandler* m_WM;

		std::queue<IOCursorEvent> m_qCursorEvents;


		//Mouse camera control
		float fMouseCtrlCamZ;

		//Mouse cursor control
		Cursor m_CursorCtrlSelectedCur;






	};
}

#endif // IOMANAGER_HPP_INCLUDED
