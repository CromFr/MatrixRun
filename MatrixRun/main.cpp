

#include <iostream>

#include <irrlicht.h>

#include <SPK.h>



#include "lib/ConfigFile.hpp"
#include "lib/Async.hpp"

#include "res.hpp"
#include "game/VfxManager.hpp"

#include "io/IOManager.hpp"

#include "game/World.hpp"
#include "game/Sentinel.hpp"
#include "game/Bullet.hpp"




int main()
{
	using namespace std;
	using namespace irr;
/*
	ofstream logFile("log.txt");
	streambuf* streambufCout = cout.rdbuf();
	streambuf* streambufLog = logFile.rdbuf();

	cout.rdbuf(streambufLog);
*/
	cout<<" /////////////////////////////////////////////////"<<endl
        <<"O===============================================O/"<<endl
        <<"|                  Matrix-Run!                  |/"<<endl
        <<"|          Réalisé par Thibaut CHARLES          |/"<<endl
        <<"|              (tcharl15@iseb.fr)               |/"<<endl
        <<"O===============================================O"<<endl;

	struct timeval timeStartLoading;
	gettimeofday(&timeStartLoading, NULL);


	cout<<endl<<"========================> Parsing config file"<<endl;
    ConfigFile Config;
    ConfigFile::Error e = Config.Load("data/config.cfg");
    if(e != ConfigFile::no_error)
	{
		cout<<"Error loading config file: '"<<ConfigFile::GetErrorString(e)<<endl;
		return 0;
	}


	cout<<endl<<"========================> Launch libAsync thread"<<endl;
	Async::GetInstance()->LaunchThread(0.02);


	cout<<endl<<endl
        <<" /////////////////////////////////"<<endl
        <<"O===============================O/"<<endl
        <<"|    Chargement du moteur 3D    |/"<<endl
        <<"O===============================O"<<endl;

	bool bFullscreen = Config.GetValue<bool>("Fullscreen");
    int nScreenWidth = Config.GetValue<int>("ScreenResolution", 0);
    int nScreenHeight = Config.GetValue<int>("ScreenResolution", 1);


    cout<<endl<<"========================> Entrées/Sorties"<<endl;
    mrio::IOManager::InputDevice inputdevCamera, inputdevCursor;

    string sControlCamera(Config.GetStringValue("CameraControlType"));
    if(sControlCamera == "mouse")inputdevCamera=mrio::IOManager::mouse;
    else if(sControlCamera == "keyboard")inputdevCamera=mrio::IOManager::keyboard;
    else if(sControlCamera == "wiimote")inputdevCamera=mrio::IOManager::wiimote;
    else cerr<<"/!\\ Error in config file : line CameraControlType, unknown value \""<<sControlCamera<<"\""<<endl;

    string sControlCursor(Config.GetStringValue("CursorControlType"));
    if(sControlCursor == "mouse")inputdevCursor=mrio::IOManager::mouse;
    else if(sControlCursor == "keyboard")inputdevCursor=mrio::IOManager::keyboard;
    else if(sControlCursor == "wiimote")inputdevCursor=mrio::IOManager::wiimote;
    else cerr<<"/!\\ Error in config file : line CameraControlType, unknown value \""<<sControlCursor<<"\""<<endl;

    mrio::IOManager IOMgr(inputdevCamera, inputdevCursor, core::vector2di(nScreenWidth, nScreenHeight));


	cout<<endl<<"========================> Device, Driver, Scene mgr, Collision mgr"<<endl;
	//Device
    IrrlichtDevice *oDev = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(nScreenWidth,nScreenHeight), 32, bFullscreen, true, Config.GetValue<bool>("VSync"), IOMgr.GetEventReceiver());
    oDev->setWindowCaption(L"Matrix-Run!   by Thibaut CHARLES");
    oDev->getCursorControl()->setVisible(true);

	//Driver
    video::IVideoDriver* oDriver = oDev->getVideoDriver();

	//SceneManager
    scene::ISceneManager *oSM = oDev->getSceneManager();

    //Collision manager
    scene::ISceneCollisionManager* oCollM = oSM->getSceneCollisionManager();


	cout<<endl<<"========================> Spark particle engine"<<endl;
    SPK::randomSeed = oDev->getTimer()->getRealTime();


	cout<<endl<<"========================> Ressources (material, models, ...)"<<endl;
	res::material::LoadDir(oDriver, "data/material");
	res::model::LoadDir(oSM, "data/model");

	game::VfxManager::Init(oDev);


    cout<<endl<<endl
        <<" /////////////////////////////////"<<endl
        <<"O===============================O/"<<endl
        <<"|    Chargement de la scène     |/"<<endl
        <<"O===============================O"<<endl;

	cout<<endl<<"========================> Caméra"<<endl;
    scene::ISceneNode* nodeCamContainer = oSM->addEmptySceneNode();

    scene::ICameraSceneNode* nodeCamera = oSM->addCameraSceneNode(nodeCamContainer, core::vector3df(0, 0, -100), core::vector3df(0, 0, 10));
    nodeCamera->bindTargetAndRotation(false);

    nodeCamera->setFarValue(3000);


	cout<<endl<<"========================> Ambiance"<<endl;

    //Fog
    oDriver->setFog(video::SColor(0, 0, 0, 0), video::EFT_FOG_LINEAR, 2000.0f, 3000.0f, 0.00001f, true, true);

    //Ambiant light
    oSM->setAmbientLight(video::SColor(128,255,255,255));

    scene::ILightSceneNode* light = oSM->addLightSceneNode(nodeCamera, core::vector3df(0,0,0), video::SColor(255,0,127,255), 600.0f);
    light->enableCastShadow();


    cout<<endl<<"========================> Scene nodes"<<endl;

    //World
    game::World oWorld(oSM);

    //Player



	struct timeval timeEndLoading;
	gettimeofday(&timeEndLoading, NULL);
	struct timeval timeLoading;
	//appying operator -
	{
		int nMod = 0;
		timeLoading.tv_usec = timeEndLoading.tv_usec - timeStartLoading.tv_usec;
		if(timeLoading.tv_usec<0)
		{
			nMod = 1;
			timeLoading.tv_usec=1000000-timeLoading.tv_usec;
		}
		timeLoading.tv_sec = timeEndLoading.tv_sec - timeStartLoading.tv_sec - nMod;
	}
	cout<<endl<<"O===============================O"<<endl
				<<"Finished in "<<timeLoading.tv_sec<<"."<<timeLoading.tv_usec/1000000<<" seconds"<<endl<<endl;


    //Boucle principale
    while(oDev->run())
    {
        //Prépare le rendu
        oDriver->beginScene(true, true, 0);//la scène a un fond noir

        /*====================================================================================================================
        \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
         >> DEBUT DES CALCULS ! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ====================================================================================================================*/


        /*==========================================================================================
        \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
        // Gestion de la caméra /////////////////////////////////////////////////////////////////////
        ==========================================================================================*/

		//On récupère les coordonnées du joueur
		bool bCameraEnoughIRSrc;
		core::vector3df posCamera = IOMgr.GetCameraPosition(&bCameraEnoughIRSrc);

		//cout<<posCamera.X<<"\t"<<posCamera.Y<<"\t"<<posCamera.Z<<endl;

		//On positionne la caméra à l'endroit du joueur
		nodeCamContainer->setPosition(posCamera);


		//==================== On calcule le champs de vision
		//Horizontalement
		float fHrzFOV1=atan((WORLD_WIDTH/2+posCamera.X)/posCamera.Z);
		float fHrzFOV2=atan((-WORLD_WIDTH/2+posCamera.X)/posCamera.Z);
		float fHrzFOV = fabs(fHrzFOV1-fHrzFOV2);
		// ...

		//Verticalement
		float fVerFOV1=atan((WORLD_HEIGHT/2+posCamera.Y)/posCamera.Z);
		float fVerFOV2=atan((-WORLD_HEIGHT/2+posCamera.Y)/posCamera.Z);
		float fVerFOV = fabs(fVerFOV1-fVerFOV2);

		nodeCamera->setAspectRatio(fHrzFOV/fVerFOV);
		nodeCamera->setFOV(fHrzFOV);
		//--------------------


		core::vector3df facFacing((abs(fVerFOV1)-fVerFOV)/2.0, -(abs(fHrzFOV1)-fHrzFOV)/2.0, 0);


        /*==========================================================================================
        \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
        // Gestion des events curseur droit /////////////////////////////////////////////////////////
        ==========================================================================================*/


        while( IOMgr.GetIsCursorEvent(mrio::IOManager::right) )
		{
			const struct mrio::IOManager::IOCursorEvent e = IOMgr.GetLastCursorEvent(mrio::IOManager::right);
			if(e.button == mrio::IOManager::primary && e.event == mrio::IOManager::pressed)
			{
				core::line3df ray(oCollM->getRayFromScreenCoordinates(e.pos, nodeCamera));

				new game::Bullet(&oWorld, oSM, nodeCamContainer->getAbsolutePosition()-oWorld.getAbsolutePosition(), ray.getVector(), 1500.0);
			}
			else if(e.button == mrio::IOManager::secondary)
			{

			}
			else if(e.button == mrio::IOManager::menu)
			{

			}
		}

		//Rendu des scene nodes
        oSM->drawAll();


        /*====================================================================================================================
        \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
         >> ELEMENTS GUI ! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ====================================================================================================================*/

        //Notification GUI IRSrc lunettes
        if(!bCameraEnoughIRSrc)
            oDriver->draw2DImage(res::material::Get("no_ir_src.png"), core::vector2di(50,50),
                        core::rect<s32>(0,0,200,200), 0,
                        video::SColor(255, 255, 255, 255), true);


        //Dessin des curseurs
        bool bEnoughIRSrc = true;
        core::vector2di vLeftCursorPos(IOMgr.GetCursorPosition(mrio::IOManager::left, &bEnoughIRSrc));
        if(bEnoughIRSrc)
            oDriver->draw2DImage(res::material::Get("leftcrosshair.png"), vLeftCursorPos - core::vector2di(32, 32),
                        core::rect<s32>(0,0,64,64), 0,
                        video::SColor(255, 255, 255, 255), true);

        core::vector2di vRightCursorPos(IOMgr.GetCursorPosition(mrio::IOManager::right, &bEnoughIRSrc));
        if(bEnoughIRSrc)
            oDriver->draw2DImage(res::material::Get("rightcrosshair.png"), vRightCursorPos - core::vector2di(32, 32),
                        core::rect<s32>(0,0,64,64), 0,
                        video::SColor(255, 255, 255, 255), true);



        //Affichage de la scène
        oDriver->endScene();

    }

    oDev->drop();
}


