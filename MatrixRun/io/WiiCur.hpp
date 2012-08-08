#ifndef WiiCur_HPP_INCLUDED
#define WiiCur_HPP_INCLUDED

#include <cmath>
#include <queue>
using namespace std;

#include <wiiuse.h>
#include "../lib/ConfigFile.hpp"



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


enum WiiCurEvent
{
    EVENT_NONE,
    EVENT_PRESSED,
    EVENT_RELEASED,
};

struct WiimoteCursorEvent
{
    WiiCurEvent event;
    bool outofscr;
    Wiimote2dPercent pos;
    int button;
};

class WiiCur
{
    public:

    ///Constructeur
    /// @param WMTable Tableau des wiimotes
    /// @param Wiimote Pointeur sur la WM qui servira de positionnement
    /// @param nConnectedWM Nombre de WM connectees
    /// @param Config Fichier de configuration � utiliser pour r�cup�rer/ecrire les infos & donnes de calibration.
    WiiCur(wiimote** WMTable, wiimote* Wiimote, int nConnectedWM, ConfigFile* Config);

    ///R�cup�re la position du dernier clic de souris plac� dans une file d'attente
    /// @return La position du curseur en pourcentage suivant x et y si il y a un clic en file d'attente, 0 sinon
    Wiimote2dPercent GetCursorPos()const;


    ///Rel�ve la derni�re action effectu�e
    /// @return la struct contenant l'ID du bouton, l'action effectuee et la position vis�e
    /// @warning Supprime imm�diatement le clic de la file d'attente
    struct WiimoteCursorEvent GetLastButtonEvent();


    ///V�rifie si tel bouton est enfonc�
    /// @param nButton Bouton � v�rifier
    /// @return true si le bouon est enfonc�
    bool GetIsButtonHeld(int nButton);



    ///Mise a jour des donn�es de position du curseur
    void UpdateCursor();

    ///Renvoie la wiimote utilisee
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

#endif // WiiCur_HPP_INCLUDED
