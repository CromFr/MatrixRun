#include "WiiPos.hpp"
using namespace std;

namespace mrio
{
	namespace wm
	{

/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
Wiimote3d::Wiimote3d(float x, float y, float z)
{
    Wiimote3d::x=x;
    Wiimote3d::y=y;
    Wiimote3d::z=z;
}

void Wiimote3d::set(float x, float y, float z)
{
    Wiimote3d::x=x;
    Wiimote3d::y=y;
    Wiimote3d::z=z;
}





/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
WiiPos::WiiPos(wiimote** WMTable, wiimote* Wiimote, int nConnectedWM, ConfigFile* Config, float fPitch, bool bConfigure)
{
	if(Wiimote == 0)
	{
		cerr<<"Critical error when initializing a WiiPos : null pointer to selected wiimote"<<endl;
		return;
	}
    m_WMTable = WMTable;
    m_WM = Wiimote;
    m_Config = Config;
    m_nConnectedWM = nConnectedWM;


    m_bEnoughSrc = false;


    //Configuration de la wiimote fixe
    wiiuse_set_ir(m_WM, true);

    //===========================================> Calibrage !
    char cAns;

    //Recherche des donnees de calibrage

    try
    {
        m_Config->Print();

		/*
        istringstream* iss;
        iss=new istringstream(Config->GetValue("DistDotToDotmm"));         *iss>>m_fCALDistDotToDotmm;          delete iss;
        iss=new istringstream(Config->GetValue("DistDotToDotpx"));         *iss>>m_fCALDistDotToDotpx;          delete iss;
        iss=new istringstream(Config->GetValue("DistWmToDotsmm"));         *iss>>m_fCALDistWmToDotsmm;          delete iss;
        iss=new istringstream(Config->GetValue("DistWmToScreenCenter"));   *iss>>m_fCALDistWmToScreenCenter;    delete iss;
        iss=new istringstream(Config->GetValue("Ratio"));                  *iss>>m_fCALRatio;                   delete iss;
        iss=new istringstream(Config->GetValue("Scale"));                  *iss>>m_fCALScale;                   delete iss;
        iss=new istringstream(Config->GetValue("Pitch"));                  *iss>>m_fPitch;                   delete iss;
        */

        m_fCALDistDotToDotmm = Config->GetValue<float>("DistDotToDotmm");
        m_fCALDistDotToDotpx = Config->GetValue<float>("DistDotToDotpx");
        m_fCALDistWmToDotsmm = Config->GetValue<float>("DistWmToDotsmm");
        m_fCALDistWmToScreenCenter = Config->GetValue<float>("DistWmToScreenCenter");
        m_fCALRatio = Config->GetValue<float>("Ratio");
        m_fCALScale = Config->GetValue<float>("Scale");
        m_fPitch = Config->GetValue<float>("Pitch");

        m_fCosPitch = cos(3.14159265*fPitch/180);
        m_fSinPitch = sin(3.14159265*fPitch/180);


        if(bConfigure)
        {
            cout<<endl<<endl<<endl<<endl
                <<" /////////////////////////////////////////////////"<<endl
                <<"O===============================================O/"<<endl
                <<"| Voulez vous calibrer la Wiimote ? O/N         |/"<<endl
                <<"|  DotToDot = "<<m_fCALDistDotToDotmm<<"mm = "<<m_fCALDistDotToDotpx<<"px\t\t\t|/"<<endl
                <<"|  WmToDot = "<<m_fCALDistWmToDotsmm<<"mm\tWmToScreen = "<<m_fCALDistWmToScreenCenter<<"\t|/"<<endl
                <<"|  Ratio = "<<m_fCALRatio<<"\tScale = "<<m_fCALScale<<"\t|/"<<endl
                <<"O===============================================O"<<endl;

            cin>>cAns;
        }
        else
            cAns='n';
    }
    catch(int ex)
    {
        cout<<endl<<endl<<endl<<endl
            <<" /////////////////////////////////////"<<endl
            <<"O===================================O/"<<endl
            <<"| 'calib.conf' non trouve !         |/"<<endl
            <<"| Vous devez effectuer le calibrage |/"<<endl
            <<"O===================================O/"<<endl;

        //A la question "voulez vous calibrer", la réponse sera non
        cAns='o';
    }

    if(cAns=='n' || cAns=='N')
    {
        //ne rien faire : les données sont déja enregistrées
    }
    else
    {
        Calibrer();
    }


    cout<<endl<<"Wiimote fixe calibrée et prête"<<endl;
}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void WiiPos::Calibrer()
{
    cout<<endl<<"<== Distance entre les deux sources infrarouges (mm) : ";
    cin>>m_fCALDistDotToDotmm;

    cout<<endl<<"<== Distance entre la wiimote et les sources infrarouges (mm) : ";
    cin>>m_fCALDistWmToDotsmm;

    cout<<endl<<"<== Inclinaison de la wiimote = Pitch (degré) : ";
    cin>>m_fPitch;


    cout<<endl<<"<== Positionnez les sources et appuyez sur (A)..."<<endl;

    bool bLoop = true;
    while(bLoop)
    {
        if(wiiuse_poll(m_WMTable, m_nConnectedWM))
        {
            switch(m_WM->event)
            {
                case WIIUSE_EVENT:
                    if(IS_JUST_PRESSED(m_WM, WIIMOTE_BUTTON_A))
                    {
                        if(m_WM->ir.dot[0].visible && m_WM->ir.dot[1].visible)
                        {
                            //Calibration !

                            m_fCALDistDotToDotpx = sqrt(   (m_WM->ir.dot[0].x-m_WM->ir.dot[1].x)
                                                             *(m_WM->ir.dot[0].x-m_WM->ir.dot[1].x)
                                                            +
                                                              (m_WM->ir.dot[0].y-m_WM->ir.dot[1].y)
                                                             *(m_WM->ir.dot[0].y-m_WM->ir.dot[1].y) );


                            m_fCALRatio = m_fCALDistWmToDotsmm * m_fCALDistDotToDotpx;

                            m_fCALScale = m_fCALDistDotToDotmm / m_fCALDistDotToDotpx;

                            cout<<"==> Wiimote calibree !"<<endl;

							/*
                            string tmp;
                            tmp=m_fCALDistDotToDotmm;        m_Config->SetValue("DistDotToDotmm", tmp);
                            tmp=m_fCALDistDotToDotpx;        m_Config->SetValue("DistDotToDotpx", tmp);
                            tmp=m_fCALDistWmToDotsmm;        m_Config->SetValue("DistWmToDotsmm", tmp);
                            tmp=m_fCALDistWmToScreenCenter;  m_Config->SetValue("DistWmToScreenCenter", tmp);
                            tmp=m_fCALRatio;                 m_Config->SetValue("Ratio", tmp);
                            tmp=m_fCALScale;                 m_Config->SetValue("Scale", tmp);
                            tmp=m_fPitch;                 m_Config->SetValue("Pitch", tmp);
                            */

        					m_Config->SetValue<float>("DistDotToDotmm", m_fCALDistDotToDotmm);
							m_Config->SetValue<float>("DistDotToDotpx", m_fCALDistDotToDotpx);
							m_Config->SetValue<float>("DistWmToDotsmm", m_fCALDistWmToDotsmm);
							m_Config->SetValue<float>("DistWmToScreenCenter", m_fCALDistWmToScreenCenter);
							m_Config->SetValue<float>("Ratio", m_fCALRatio);
							m_Config->SetValue<float>("Scale", m_fCALScale);
							m_Config->SetValue<float>("Pitch", m_fPitch);

                            bLoop=false;
                        }
                        else
                            cout<<"/!\\ La calibration requiers deux sources infrarouges..."<<endl;
                    }
                    break;

                default:
                    break;
            }
        }
    }
}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
Wiimote3d WiiPos::GetPosition() const
{
    if(m_bEnoughSrc == false)
        throw EXC_WIIPOS_NOT_ENOUGH_IRSRC;
    else
    {
        return m_pos;
    }
}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void WiiPos::UpdatePosition()
{
    if(m_WM->ir.dot[0].visible && m_WM->ir.dot[1].visible)
    {
        m_bEnoughSrc = true;

        Wiimote3d posA, posB, posC;

        //Positon des dots
        posA.x = m_WM->ir.dot[0].x;
        posA.y = 768-m_WM->ir.dot[0].y;
        posB.x = m_WM->ir.dot[1].x;
        posB.y = 768-m_WM->ir.dot[1].y;

        //Position du centre
        posC.x = (posA.x+posB.x)/2;
        posC.y = (posA.y+posB.y)/2;

        //Calcul de la distance en px séparant les dots
        float fDistDotToDot = sqrt( (m_WM->ir.dot[0].x-m_WM->ir.dot[1].x)
                                     *(m_WM->ir.dot[0].x-m_WM->ir.dot[1].x)
                                    +
                                      (m_WM->ir.dot[0].y-m_WM->ir.dot[1].y)
                                     *(m_WM->ir.dot[0].y-m_WM->ir.dot[1].y) );


        //float fPitch = (-m_WM->orient.pitch/180)*3.14159265;//;
        //cout<<fPitch<<endl;

        float fY0=(-m_fCALRatio / fDistDotToDot);
        float fX0=-(posC.x-512) * m_fCALScale * fY0 / m_fCALDistWmToDotsmm;
        float fZ0= ((posC.y-384) * m_fCALScale * fY0 / m_fCALDistWmToDotsmm);

        //Gestion du pitch
        float fY1=m_fCosPitch*fY0 + m_fSinPitch*fZ0;
        float fX1=fX0;
        float fZ1=-m_fSinPitch*fY0 + m_fCosPitch*fZ0 + m_fCALDistWmToScreenCenter;

        //cout<<"X="<<fX<<"\tY="<<fY<<"\tZ="<<fZ<<endl;

        m_pos.set(fX1, fY1+600, fZ1);
    }
    else
        m_bEnoughSrc = false;


}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
wiimote* WiiPos::GetWiimote()const
{
    return m_WM;
}



	}
}
