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
		@example Using the events
		mrio::IOManager IOMgr(...);
		(...)
		while( IOMgr.GetIsCursorEvent() )
		{
			const struct mrio::IOManager::IOCursorEvent e = IOMgr.GetLastCursorEvent();

			//Treating the event...
		}
		**/
		bool GetIsCursorEvent();

		const IOCursorEvent& GetLastCursorEvent();

		irr::IEventReceiver* GetEventReceiver(){return m_StdIn;}


		void SwitchCursor(float fWheelDir);



	private:
		InputDevice m_CamCtrl;
		InputDevice m_CurCtrl;
		irr::core::vector2di m_vScreenDim;

		IOCursorEvent m_LastCursorEvent;
		irr::core::vector3df m_vLastCameraPos;
		irr::core::vector2di m_vLastLeftCursorPos;
		irr::core::vector2di m_vLastRightCursorPos;

		StdInput* m_StdIn;
		wm::WiimoteHandler* m_WM;

		queue<IOCursorEvent> m_qCursorEvents;


		//Mouse camera control
		float fMouseCtrlCamZ;

		//Mouse cursor control
		Cursor m_CursorCtrlSelectedCur;






	};
}

#endif // IOMANAGER_HPP_INCLUDED
