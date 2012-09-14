#ifndef WIIMOTEHANDLER_HPP_INCLUDED

#define WIIMOTEHANDLER_HPP_INCLUDED


#include <iostream>

#include <boost/thread.hpp>

#include <wiiuse.h>



#include "../lib/ConfigFile.hpp"
#include "WiiPos.hpp"
#include "WiiCur.hpp"


#define NB_WIIMOTES 4

#define WMHDL_LEFT 0
#define WMHDL_RIGHT 1


namespace mrio
{
	namespace wm
	{
		/**
		@brief Handle class to manage all wiimotes
		**/
		class WiimoteHandler
		{
		public:
			///Organise le tableau des wiimotes & entame la procédure d'initialisation
			WiimoteHandler();

			~WiimoteHandler();


			/**
			@brief Access to WiiPos::GetPosition and return the position of the player
			@throws EXC_WIIPOS_NOT_ENOUGH_IRSRC if there are less than 2IR sources
			**/
			Wiimote3d GetPlayerPos()const;

			/**
			@brief Access to WiiCur::GetCursorPos and return the position of the cursor in % of the screen
			@throws EXC_WIICUR_NO_IRSRC if no IR sources are found
			@throws EXC_WIICUR_OUT_OF_SCREEN if the cursor is out of the screen
			@param nWM WMHDL_LEFT or WMHDL_RIGHT to select which WM is concerned
			**/
			Wiimote2dPercent GetCursorPos(int nWM)const;

			/**
			@brief Access to WiiCur::GetLastButtonEvent : Relève la dernière action effectuée sur la WM
			@return la struct contenant tout ce qu'il faut pour l'event
			**/
			struct WiimoteCursorEvent GetLastButtonEvent(int nWM);

			void DropLastButtonEvent(int nWM)
			{
				if(nWM==WMHDL_RIGHT)
					return m_WiimoteRight->DropLastButtonEvent();
				return m_WiimoteLeft->DropLastButtonEvent();
			}

			bool GetIsButtonEvent(int nWM)
			{
				if(nWM==WMHDL_RIGHT)
					return m_WiimoteRight->GetIsButtonEvent();
				return m_WiimoteLeft->GetIsButtonEvent();
			}





		private:
			wiimote** m_WMTable;
			int m_nConnectedWM;
			ConfigFile m_Config;

			WiiPos* m_WiimotePos;
			WiiCur* m_WiimoteRight;
			WiiCur* m_WiimoteLeft;


			//Threading
			void Update();
			boost::thread* UpdatingThread;
			bool m_bStopThread;
			bool m_bPauseThread;


			void ClearWiimoteEvents();

		};
	}
}



#endif
