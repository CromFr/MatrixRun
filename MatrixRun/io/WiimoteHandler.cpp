#include "WiimoteHandler.hpp"



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
WiimoteHandler::WiimoteHandler()
{

	if(m_Config.Load("data/wiimotes.cfg", false) == ConfigFile::file_not_found)
	{
		cerr<<"/!\\ WiimoteHandler : configuration file not found !!!!!!!!"<<endl;
		return;
	}

	bool bConfigure = m_Config.GetValue<int>("RunWMConfig");
	cout<<"bConfigure="<<bConfigure<<endl;


    m_bStopThread = false;
    m_bPauseThread = false;


    #ifndef NO_WIIMOTES

    cout<<"==> Initialisation du tableau des wiimotes"<<endl;
    m_WMTable = wiiuse_init(NB_WIIMOTES);


    cout<<"==> Recherche des wiimotes..."<<endl;
    int nFound = wiiuse_find(m_WMTable, NB_WIIMOTES, 5);
    cout<<" => "<<nFound<<" wiimote(s) trouvee(s)"<<endl;
    if(nFound==0)
    {
        cout<<endl<<endl<<"IMPOSSIBLE DE CONTINUER"<<endl;
        while(1);
    }

    cout<<"==> Connection des wiimotes..."<<endl;
    m_nConnectedWM = wiiuse_connect(m_WMTable, nFound);
    cout<<" => "<<m_nConnectedWM<<" wiimote(s) connectee(s)"<<endl;
    if(m_nConnectedWM==0)
    {
        cout<<endl<<endl<<"IMPOSSIBLE DE CONTINUER"<<endl;
        while(1);
    }


	//On d�sactive tous les modules des WM, ils seront r�activ�e manuellement ensuite
	for(int i=0 ; i<nFound ; i++)
	{
		//wiiuse_set_flags(m_WMTable[i], WIIUSE_INIT_FLAGS);
		wiiuse_motion_sensing(m_WMTable[i], false);
		wiiuse_set_accel_threshold(m_WMTable[i], false);
		//wiiuse_set_ir_sensitivity(m_WMTable[i], 1);
	}



	if(bConfigure)
	{


		//R�cup des infos de la batterie & clear des events wiimote
		int nGotBattLvls = 0;
		while(nGotBattLvls<m_nConnectedWM)
		{
			while(wiiuse_poll(m_WMTable, nFound))
			{
				int n;
				for(n=0 ; n < nFound; ++n)
				{
					float fBattery;
					switch(m_WMTable[n]->event)
					{
						case WIIUSE_STATUS:
							fBattery = 100*m_WMTable[n]->battery_level;

							if(fBattery>=75)
								wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3 | WIIMOTE_LED_4);
							else if(fBattery>=50)
								wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_2 | WIIMOTE_LED_3);
							else if(fBattery>=25)
								wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_1 | WIIMOTE_LED_2);
							else
								wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_1);

							nGotBattLvls++;
							break;
						default:
							break;
					}
				}
			}
		}


		//==================================================================================================================
		cout<<endl<<endl<<endl<<endl
			<<" ////////////////////////////////////////////////"<<endl
			<<"O==============================================O/"<<endl
			<<"| Appuyez sur le bouton (A) de la wiimote fixe |/"<<endl
			<<"O==============================================O"<<endl;
		bool bLoop = true;
		while(bLoop)
		{
			if(wiiuse_poll(m_WMTable, nFound))
			{
				int n;
				for(n=0 ; n < nFound; ++n)
				{
					switch(m_WMTable[n]->event)
					{
						case WIIUSE_EVENT:
							if(IS_JUST_PRESSED(m_WMTable[n], WIIMOTE_BUTTON_A))
							{
								cout<<"==> WiiMote fixe trouvee"<<endl;

								wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_2 | WIIMOTE_LED_3);

								//Enregistrement dans la config
								m_Config.SetStringValue("MACAddrFixedWM", m_WMTable[n]->bdaddr_str);
								cout<<"    Addr MAC="<<m_WMTable[n]->bdaddr_str<<endl;

								//cr�ation & configuration de la WiiPos
								// TODO (Administrateur#1#): Use dynamic detection of pitch
								m_WiimotePos = new WiiPos(m_WMTable, m_WMTable[n], m_nConnectedWM, &m_Config, 26.0, bConfigure);

								bLoop=false;
							}
							break;
						default:
							break;
					}
				}
			}
		}

		ClearWiimoteEvents();
		//==================================================================================================================
		cout<<endl<<endl<<endl<<endl
			<<" /////////////////////////////////////////////////////"<<endl
			<<"O===================================================O/"<<endl
			<<"| Appuyez sur le bouton (+) de la wiimote de droite |/"<<endl
			<<"O===================================================O"<<endl;
		bLoop = true;
		while(bLoop)
		{
			if(wiiuse_poll(m_WMTable, nFound))
			{
				int n;
				for(n=0 ; n < nFound; ++n)
				{
					if(m_WMTable[n] == m_WiimotePos->GetWiimote())
						continue;

					switch(m_WMTable[n]->event)
					{
						case WIIUSE_EVENT:
							if(IS_JUST_PRESSED(m_WMTable[n], WIIMOTE_BUTTON_PLUS))
							{
								cout<<"==> WiiMote de droite trouvee et prete"<<endl;
								wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_3 | WIIMOTE_LED_4);

								//Enregistrement dans la config
								m_Config.SetStringValue("MACAddrLeftWM", m_WMTable[n]->bdaddr_str);
								cout<<"    Addr MAC="<<m_WMTable[n]->bdaddr_str<<endl;

								m_WiimoteRight = new WiiCur(m_WMTable, m_WMTable[n], m_nConnectedWM, &m_Config);

								bLoop=false;
							}
							break;

						default:
							break;
					}
				}
			}
		}
		ClearWiimoteEvents();
		/*
		//==================================================================================================================
		cout<<endl<<endl<<endl<<endl
			<<" /////////////////////////////////////////////////////"<<endl
			<<"O===================================================O/"<<endl
			<<"| Appuyez sur le bouton (-) de la wiimote de gauche |/"<<endl
			<<"O===================================================O"<<endl;
		bLoop = true;
		while(bLoop)
		{
			if(wiiuse_poll(m_WMTable, nFound))
			{
				int n;
				for(n=0 ; n < nFound; ++n)
				{
					if(m_WMTable[n] == m_WiimotePos->GetWiimote() || m_WMTable[n] == m_WiimoteRight->GetWiimote())
						continue;

					switch(m_WMTable[n]->event)
					{
						case WIIUSE_EVENT:
							if(IS_JUST_PRESSED(m_WMTable[n], WIIMOTE_BUTTON_MINUS))
							{
								cout<<"==> WiiMote de droite trouvee et prete"<<endl;
								wiiuse_set_leds(m_WMTable[n], WIIMOTE_LED_3 | WIIMOTE_LED_4);

								//Enregistrement dans la config
								m_Config.SetStringValue("MACAddrRightWM", m_WMTable[n].bdaddr_str);
								cout<<"    Addr MAC="<<m_WMTable[n]->bdaddr_str<<endl;

								m_WiimoteLeft = new WiiCur(m_WMTable, m_WMTable[n], m_nConnectedWM, Config, bConfigure);

								bLoop=false;
							}
							break;

						default:
							break;
					}
				}
			}
		}*/

		m_Config.WriteToFile("data/wiimotes.cfg");
	}
	else
	{
		ClearWiimoteEvents();
		bool bFound[3]={false, false, false};
		for(int i=0 ; i<nFound ; i++)
		{
			string sAddr(m_WMTable[i]->bdaddr_str);
			if(sAddr == m_Config.GetStringValue("MACAddrFixedWM"))
			{
				bFound[0] = true;
				wiiuse_set_leds(m_WMTable[i], WIIMOTE_LED_2 | WIIMOTE_LED_3);
				m_WiimotePos = new WiiPos(m_WMTable, m_WMTable[i], m_nConnectedWM, &m_Config, m_Config.GetValue<float>("Pitch"), bConfigure);
			}
			else if(sAddr == m_Config.GetStringValue("MACAddrLeftWM"))
			{
				bFound[1] = true;
				wiiuse_set_leds(m_WMTable[i], WIIMOTE_LED_1 | WIIMOTE_LED_2);
				m_WiimoteRight = new WiiCur(m_WMTable, m_WMTable[i], m_nConnectedWM, &m_Config);
			}
			else if(sAddr == m_Config.GetStringValue("MACAddrRightWM"))
			{
				bFound[2] = true;
				wiiuse_set_leds(m_WMTable[i], WIIMOTE_LED_3 | WIIMOTE_LED_4);
				m_WiimoteLeft = new WiiCur(m_WMTable, m_WMTable[i], m_nConnectedWM, &m_Config);
			}
		}

		if(!bFound[0])cerr<<"/!\\  Attention : Wiimote fixe non trouv�e (MAC="<<m_Config.GetStringValue("MACAddrFixedWM")<<")"<<endl;
		if(!bFound[1])cerr<<"/!\\  Attention : Wiimote gauche non trouv�e (MAC="<<m_Config.GetStringValue("MACAddrLeftWM")<<")"<<endl;
		if(!bFound[2])cerr<<"/!\\  Attention : Wiimote droite non trouv�e (MAC="<<m_Config.GetStringValue("MACAddrRightWM")<<")"<<endl;

	}



    //Lancement du thread d'update
    UpdatingThread = new boost::thread(boost::bind(&WiimoteHandler::Update, this));

    #endif
    #ifdef NO_WIIMOTES

    cout<<endl<<endl<<endl<<endl
        <<" /////////////////////////////////"<<endl
        <<"O===============================O/"<<endl
        <<"|    WIIMOTES DESACTIVEES !!!   |/"<<endl
        <<"O===============================O"<<endl;
    #endif


}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
WiimoteHandler::~WiimoteHandler()
{
    #ifndef NO_WIIMOTES

    delete m_WiimotePos;
    delete m_WiimoteRight;
    //delete m_WiimoteLeft;

    delete UpdatingThread;

    wiiuse_cleanup(m_WMTable, NB_WIIMOTES);

    #endif
};



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
Wiimote3d WiimoteHandler::GetPlayerPos()const
{
    #ifndef NO_WIIMOTES
    try
    {
        return m_WiimotePos->GetPosition();
    }
    catch(int e)
    {
        throw e;
    }
    throw -1;
    #endif


    #ifdef NO_WIIMOTES
    return Wiimote3d(0, -400, 0);
    #endif
}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
Wiimote2dPercent WiimoteHandler::GetCursorPos(int nWM)const
{
    #ifndef NO_WIIMOTES
    try
    {
        if(nWM==WMHDL_RIGHT)
            return m_WiimoteRight->GetCursorPos();
        else if(nWM==WMHDL_LEFT)
            return m_WiimoteLeft->GetCursorPos();
    }
    catch(int e)
    {
        throw e;
    }
    throw -1;
    #endif


    #ifdef NO_WIIMOTES
    return Wiimote2dPercent(0.0, 0.0);
    #endif
}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
struct WiimoteCursorEvent WiimoteHandler::GetLastButtonEvent(int nWM)
{
    #ifndef NO_WIIMOTES
    if(nWM==WMHDL_RIGHT)
        return m_WiimoteRight->GetLastButtonEvent();
    //si c'est pas la right c'est la left
    return m_WiimoteLeft->GetLastButtonEvent();
    #endif


    #ifdef NO_WIIMOTES
    WiimoteCursorEvent toReturn;
    toReturn.button=0;
    toReturn.event=EVENT_NONE;
    toReturn.outofscr=0;
    toReturn.pos = Wiimote2dPercent(0.0, 0.0);
    return toReturn;
    #endif
}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void WiimoteHandler::Update()
{
    #ifndef NO_WIIMOTES
    while(!m_bStopThread)
    {
        if(wiiuse_poll(m_WMTable, NB_WIIMOTES))
        {
            m_WiimotePos->UpdatePosition();
            m_WiimoteRight->UpdateCursor();
            //m_WiimoteLeft->UpdateCursor();
        }
        while(m_bPauseThread);
    }
    #endif
}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void WiimoteHandler::ClearWiimoteEvents()
{
    #ifndef NO_WIIMOTES
    while(wiiuse_poll(m_WMTable, NB_WIIMOTES));
    #endif
}








