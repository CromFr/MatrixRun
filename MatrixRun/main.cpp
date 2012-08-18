

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


	cout<<endl<<endl
        <<" /////////////////////////////////////////////////"<<endl
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

    #define FULLSCREEN false
    int nScreenWidth, nScreenHeight;
    if(FULLSCREEN)  { nScreenWidth=1920;  nScreenHeight=1080; }
    else            { nScreenWidth=1900;  nScreenHeight=1080-30-80;  }


    cout<<endl<<"========================> Entrées/Sorties"<<endl;
    mrio::IOManager IOMgr(mrio::IOManager::mouse, mrio::IOManager::keyboard, core::vector2di(nScreenWidth, nScreenHeight));


	cout<<endl<<"========================> Device, Driver, Scene mgr, Collision mgr"<<endl;
	//Device
    IrrlichtDevice *oDev = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(nScreenWidth,nScreenHeight), 32, FULLSCREEN, false, false, IOMgr.GetEventReceiver());
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
	cout<<endl<<"O===============================O"<<endl
				<<"Finished in "<<(timeStartLoading.tv_usec - timeEndLoading.tv_usec)/1000<<" ms"<<endl<<endl;

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
			if(e.button == mrio::IOManager::primary)
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
        if(bCameraEnoughIRSrc)
            oDriver->draw2DImage(res::material::Get("no_ir_src.png"), core::vector2di(50,50),
                        core::rect<s32>(0,0,200,200), 0,
                        video::SColor(255, 255, 255, 255), true);



        //Dessin du curseur droit
        bool bRightCursorEnoughIRSrc;
        core::vector2di vRightCursorPos(IOMgr.GetCursorPosition(mrio::IOManager::right, &bRightCursorEnoughIRSrc));
        if(bRightCursorEnoughIRSrc)
            oDriver->draw2DImage(res::material::Get("rightcrosshair.png"), vRightCursorPos - core::vector2di(32, 32),
                        core::rect<s32>(0,0,64,64), 0,
                        video::SColor(255, 255, 255, 255), true);



        //Affichage de la scène
        oDriver->endScene();



        //Frametime
        core::stringw sCaption = L"FPS=";
        sCaption += oDriver->getFPS();//oTimer->getTime()
        oDev->setWindowCaption(sCaption.c_str());

        sleep(0.1);
    }

    oDev->drop();
}


