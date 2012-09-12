#ifndef WiiCur_HPP_INCLUDED
#define WiiCur_HPP_INCLUDED

#include <cmath>
#include <queue>

#include <wiiuse.h>
#include "../lib/ConfigFile.hpp"


namespace mrio
{
	namespace wm
	{
		class Wiimote2dPercent
		{
			public:
			Wiimote2dPercent(double x=0, double y=0);
			void set(double x=0, double y=0);

			double x;
			double y;
		};



		#define EXC_WIICUR_NO_IRSRC 1
		#define EXC_WIICUR_OUT_OF_SCREEN 2

		#define EXC_WIICUR_NO_EVENTS 1

		/**
		@brief Etat d'un bouton suite � un event
		**/
		enum WiiCurEvent
		{
			EVENT_NONE,
			EVENT_PRESSED,
			EVENT_RELEASED,
		};

		/**
		@brief Event de la wiimote
		**/
		struct WiimoteCursorEvent
		{
			WiiCurEvent event;
			bool outofscr;
			Wiimote2dPercent pos;
			int button;
		};

		/**
		@brief Une wiimote servant de curseur
		**/
		class WiiCur
		{
		public:

			/**
			@brief Constructeur
			@param WMTable Tableau des wiimotes
			@param Wiimote Pointeur sur la WM qui servira de positionnement
			@param nConnectedWM Nombre de WM connectees
			@param Config Fichier de configuration � utiliser pour r�cup�rer/ecrire les infos & donnes de calibration.
			**/
			WiiCur(wiimote** WMTable, wiimote* Wiimote, int nConnectedWM, ConfigFile* Config);

			/**
			@brief R�cup�re la position du dernier clic de souris plac� dans une file d'attente
			@return La position du curseur en pourcentage suivant x et y si il y a un clic en file d'attente, 0 sinon
			**/
			Wiimote2dPercent GetCursorPos()const;

			/**
			@brief Rel�ve la derni�re action effectu�e
			@return la struct contenant l'ID du bouton, l'action effectuee et la position vis�e
			**/
			struct WiimoteCursorEvent GetLastButtonEvent();

			/**
			@brief Supprime le dernier event
			**/
			void DropLastButtonEvent()
			{
				m_Events.pop();
			}

			/**
			@brief Retourne 1 si il y a un evenement a traiter
			**/
			bool GetIsButtonEvent()
			{
				return m_Events.empty();
			}

			/**
			@brief V�rifie si tel bouton est enfonc�
			@param nButton Bouton � v�rifier
			@return true si le bouon est enfonc�
			**/
			bool GetIsButtonHeld(int nButton);

			/**
			@brief Mise a jour des donn�es de position du curseur
			**/
			void UpdateCursor();

			/**
			@brief Renvoie la wiimote utilisee
			**/
			wiimote* GetWiimote()const;



		private:
			wiimote* m_WM;
			wiimote** m_WMTable;
			int m_nConnectedWM;
			ConfigFile* m_Config;

			//Donn�es de position du curseur
			bool m_bIrSRC;
			bool m_bOutOfScreen;
			int m_PressedButons;
			Wiimote2dPercent m_CurPos;

			//Gestion des clics
			std::queue<WiimoteCursorEvent> m_Events;

		};

	}
}


#endif // WiiCur_HPP_INCLUDED
