#ifndef WIIPOS_HPP_INCLUDED

#define WIIPOS_HPP_INCLUDED


#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>
#include <wiiuse.h>
#include "ConfigFile.hpp"




class Wiimote3d
{
    public:
    Wiimote3d(float x=0, float y=0, float z=0);

    virtual void set(float x, float y, float z);

    //attributs
    float x;
    float y;
    float z;

};



#define EXC_WIIPOS_NOT_ENOUGH_IRSRC 1
#define EXC_WIIPOS_NO_EVENT 2

class WiiPos
{

    public:

    WiiPos(){}

    ///Constructeur
    /// @param WMTable Tableau des wiimotes
    /// @param Wiimote Pointeur sur la WM qui servira au positionnement
    /// @param nConnectedWM Nombre de WM connectees
    /// @param Config Fichier de configuration � utiliser pour r�cup�rer/ecrire les infos & donnes de calibration.
    /// @param bConfigure true si il faut configurer (calibration) la WM, false si pas necessaire
    WiiPos(wiimote** WMTable, wiimote* Wiimote, int nConnectedWM, ConfigFile* Config, float fPitch, bool bConfigure);


    void Calibrer();

    ///R�cup�re la position du joueur dans l'espace.
    /// throw EXC_WIIPOS_NOT_ENOUGH_IRSRC et EXC_WIIPOS_NO_EVENT
    /// @return La struct contenant la position du joueur en 3d
    Wiimote3d GetPosition() const;

    ///Mise a jour des donn�es de position via la r�cup des sources IR et comm via bluetooth
    void UpdatePosition();

    ///Accesseur
    /// @return La wiimote utilis�e
    wiimote* GetWiimote()const;

    private:
    wiimote* m_WM;
    wiimote** m_WMTable;
    int m_nConnectedWM;
    ConfigFile* m_Config;


    //Donn�es de position
    bool m_bEnoughSrc;
    Wiimote3d m_pos;


    //Donn�es de calibrage
    float m_fCALDistDotToDotmm;
    float m_fCALDistDotToDotpx;
    float m_fCALDistWmToDotsmm;
    float m_fCALDistWmToScreenCenter;

    float m_fCALRatio;
    float m_fCALScale;

    float m_fPitch;//deg
    float m_fCosPitch;
    float m_fSinPitch;



};




#endif
