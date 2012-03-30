#include "WiiCur.hpp"

using namespace std;


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
Wiimote2dPercent::Wiimote2dPercent(double x, double y)
{
    this->set(x, y);
}

void Wiimote2dPercent::set(double x, double y)
{
    this->x = x;
    this->y = y;
}





/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
WiiCur::WiiCur(wiimote** WMTable, wiimote* Wiimote, int nConnectedWM, ConfigFile* Config)
{
    m_WMTable = WMTable;
    m_WM = Wiimote;
    m_Config = Config;
    m_nConnectedWM = nConnectedWM;


    m_bIrSRC=false;
    m_bOutOfScreen=true;


    wiiuse_set_ir(m_WM, true);
}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
Wiimote2dPercent WiiCur::GetCursorPos()const
{
    if(m_bIrSRC == false)
        throw EXC_WIICUR_NO_IRSRC;
    else if(m_bOutOfScreen)
        throw EXC_WIICUR_OUT_OF_SCREEN;
    else
    {
        return m_CurPos;
    }
    throw -1;
}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
struct WiimoteCursorEvent WiiCur::GetLastButtonEvent()
{
    struct WiimoteCursorEvent Return;

    if(!m_Events.empty())
    {
        //Copie du dernier event & suppr
        Return = m_Events.front();
        m_Events.pop();

        return Return;
    }
    Return.button=0;Return.event=EVENT_NONE;

    return Return;

}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void WiiCur::UpdateCursor()
{
    if(m_WM->ir.dot[0].visible)
    {
        m_bIrSRC = true;

        int nIrSRC = 0;
        int nSommeX = 0;
        int nSommeY = 0;
        for(int i=0 ; i<4 ; i++)
        {
            if(m_WM->ir.dot[i].visible)
            {
                nIrSRC++;
                nSommeX += m_WM->ir.dot[i].x;
                nSommeY += m_WM->ir.dot[i].y;
            }
        }

        //Fenetre = 75% de la capacité de la wm
        // 0<x-128<768          0<y-144<576
        double fPercentX = (nSommeX/nIrSRC-128.0)/768.0;
        double fPercentY = (nSommeY/nIrSRC-144.0)/576.0;

        float fRoll = (m_WM->orient.roll/180)*3.14159265;

        float fCosRoll = cos(fRoll);
        float fSinRoll = sin(fRoll);

        //fPercentX=fCosRoll*fPercentX + fSinRoll*fPercentY;
        //fPercentY=-fSinRoll*fPercentX + fCosRoll*fPercentY;

        //fPercentY=fCosRoll*fPercentY + fSinRoll*fPercentX;
        //fPercentX=-fSinRoll*fPercentY + fCosRoll*fPercentX;

// TODO (Administrateur#1#): Gestion du roll

        m_CurPos.set(fPercentX, fPercentY);

        if(fPercentX>1 || fPercentX<0 || fPercentY>1 || fPercentY<0)
            m_bOutOfScreen=true;
        else
            m_bOutOfScreen=false;

    }
    else
        m_bIrSRC = false;


    int nButton = m_WM->btns ^ m_PressedButons;//Action sur sur nButton
    if(nButton)//Si un bouton est presse/relache
    {
        struct WiimoteCursorEvent Event;

        if(nButton & m_WM->btns)//Pressed
            Event.event=EVENT_PRESSED;
        else if(nButton & m_PressedButons)//Released
            Event.event=EVENT_RELEASED;
        else
            cout<<"ERREUR"<<endl;

        Event.button = nButton;

        Event.outofscr = m_bOutOfScreen;
        Event.pos.set(m_CurPos.x, m_CurPos.y);
        m_Events.push(Event);
    }
    m_PressedButons = m_WM->btns;

    //cout<<"Btns="<<IntToBinary(m_WM->btns, 16)<<"\tBtns_held="<<IntToBinary(m_WM->btns_held, 16)<<endl;


}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
bool WiiCur::GetIsButtonHeld(int nButton)
{
    if((m_WM->btns_held & nButton) == nButton)
        return true;
    return false;
}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
wiimote* WiiCur::GetWiimote()const
{
    return m_WM;
}












