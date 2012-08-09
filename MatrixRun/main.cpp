

#include <iostream>
#include <vector>
using namespace std;

#include <irrlicht.h>
using namespace irr;

#include <SPK.h>
#include <SPK_IRR.h>
using namespace SPK;

//L'aléatoire via boost
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "io/WiimoteHandler.hpp"
#include "io/transition.hpp"
#include "lib/ConfigFile.hpp"

#include "game/GravityAnimator.hpp"

#include "game/MRCollisionnable.hpp"








#define ID_TYPE_CIBLE 1






int main()
{
    ConfigFile Config;
    ConfigFile::Error e = Config.Load("data/config.cfg");
    if(e != ConfigFile::no_error)
	{
		cout<<"Error loading config file: '"<<ConfigFile::GetErrorString(e)<<endl;
		return 0;
	}

    WiimoteHandler WMHdl(&Config);


    #define TEST_POSITIONNING false
    {//==================== Test du syst de positionnement
    while(TEST_POSITIONNING)
    {

        try
        {
            Wiimote3d PlayerPos = WMHdl.GetPlayerPos();
            cout<<endl<<endl<<"Position :"<<endl;
            cout<<"x="<<PlayerPos.x<<"   \ty="<<PlayerPos.y<<"   \tz="<<PlayerPos.z<<endl;
        }
        catch(int e)
        {
            if(e==EXC_WIIPOS_NOT_ENOUGH_IRSRC)
                cout<<"Pas assez de sources"<<endl;
            else
                cout<<"Erreur inconnue"<<endl;
        }
        //Sleep(70);
    }
    }//--------------------




    cout<<endl<<endl<<endl<<endl
        <<" ////////////////////////////"<<endl
        <<"O==========================O/"<<endl
        <<"| Lancement du moteur 3D ! |/"<<endl
        <<"O==========================O"<<endl;


    //==================== Création de la fenêtre
    #define FULLSCREEN false
    int nScreenWidth, nScreenHeight;
    if(FULLSCREEN)  { nScreenWidth=1920;  nScreenHeight=1080; }
    else            { nScreenWidth=1900;  nScreenHeight=1080-30-80;  }
    IrrlichtDevice *oDev = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(nScreenWidth,nScreenHeight), 32, FULLSCREEN);


    oDev->setWindowCaption(L"Irrlicht : HeadTracking");
    ITimer* oTimer = oDev->getTimer();

    video::IVideoDriver* oDriver = oDev->getVideoDriver();

    scene::ISceneManager *oSM = oDev->getSceneManager ();

    //Le curseur est visible
    oDev->getCursorControl()->setVisible(true);
    //--------------------


    //==================== Chargement des images 2d
    video::ITexture *texNoIRSrc = oDriver->getTexture("data/no_ir_src.png");
    video::ITexture *texRightCrosshair = oDriver->getTexture("data/rightcrosshair.png");
    //video::ITexture *texLeftCrosshair = oDriver->getTexture("data/leftcrosshair.png");
    //--------------------


    //==================== Setup de la caméra
    scene::ISceneNode* nodeCamContainer = oSM->addEmptySceneNode();

    scene::ICameraSceneNode* nodeCamera = oSM->addCameraSceneNode(nodeCamContainer, core::vector3df(0, 0, 0), core::vector3df(0, 0, 10));
    nodeCamera->bindTargetAndRotation(false);

    nodeCamera->setFarValue(3500);
    //--------------------

    //==================== Brouillard
    oDriver->setFog(video::SColor(0, 0, 0, 0), video::EFT_FOG_LINEAR, 2000.0f, 3000.0f, 0.01f, true, true);
    //--------------------

    //==================== Eclairage
    oSM->setAmbientLight(video::SColor(0,64,64,64));

    oSM->addLightSceneNode(nodeCamera, core::vector3df(0,0,0), video::SColor(128,255,255,255), 1000.0f);
    //--------------------


    //==================== Création du cube-background
    scene::IAnimatedMeshSceneNode *nodeBackground = oSM->addAnimatedMeshSceneNode(oSM->getMesh("data/emptycube.3ds"));
    nodeBackground->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    nodeBackground->setMaterialFlag(irr::video::EMF_FOG_ENABLE, true);
    //--------------------

    //==================== Génération des cibles
    vector<scene::IAnimatedMeshSceneNode*> nodeCibles;
    vector<scene::IAnimatedMeshSceneNode*> nodeTiges;
    #ifdef FIXED_TARGETS
    boost::random::mt19937 RdmSeed;//((int)oDev->getTimer()->getRealTime());
    #else
    boost::random::mt19937 RdmSeed((int)oDev->getTimer()->getRealTime());
    #endif
    boost::random::uniform_int_distribution<> RdmXY(-135, 135);
    boost::random::uniform_int_distribution<> RdmZ(-300, 400);
    for(int i=0 ; i<20 ; i++)
    {
        int x = RdmXY(RdmSeed);
        int y = RdmXY(RdmSeed);
        int z = RdmZ(RdmSeed);

        scene::IAnimatedMeshSceneNode* nodeCible = oSM->addAnimatedMeshSceneNode(oSM->getMesh("data/cible.3ds"), 0, ID_TYPE_CIBLE, core::vector3df(x,y,z), core::vector3df(0,0,0), core::vector3df(1.5,1.5,1.5));
        nodeCible->setMaterialFlag(irr::video::EMF_LIGHTING, true);
        nodeCible->setMaterialFlag(irr::video::EMF_FOG_ENABLE, true);
        nodeCible->setTriangleSelector(oSM->createTriangleSelector(nodeCible));
        nodeCibles.push_back(nodeCible);

        nodeTiges.push_back(oSM->addAnimatedMeshSceneNode(oSM->getMesh("data/baton.3ds"), 0, -1, core::vector3df(x,y,z)));
    }
    //--------------------


    //==================== Joueur
    scene::IAnimatedMeshSceneNode *nodePlayer = oSM->addAnimatedMeshSceneNode(oSM->getMesh("data/neb/nebuchadnezzar.3ds"), 0, -1, core::vector3df(0, 0, 100), core::vector3df(0, 0, 0), core::vector3df(0.5, 0.5, 0.5));
    nodePlayer->setMaterialFlag(irr::video::EMF_LIGHTING, true);

    //              float fMasseKg, float fGravity=98, float fAirFriction=0, core::vector3df OptForce=core::vector3df(0,0,0)
    //GravityAnimator GravEngine(1000, 0, 0, core::vector3df(0, 0, 0));
    //GravEngine.FollowNode(nodeCamera, 1000, core::vector3df(0, -50, 300));
    //nodePlayer->addAnimator(&GravEngine);

    //--------------------



    //==================== Gestionnaire de collisions
    scene::ISceneCollisionManager* oCollM = oSM->getSceneCollisionManager();
    //--------------------


    //==================== Effets de particules
    #include "_ParticleEffects.cpp"
/*
    Point* ExplosionEmitZone = Point::create();

    RandomEmitter* ExplosionEmitter = RandomEmitter::create();
     ExplosionEmitter->setZone(ExplosionEmitZone);
     ExplosionEmitter->setTank(100);
     ExplosionEmitter->setFlow(400);
     ExplosionEmitter->setForce(75.0, 200.0);

    Model* ExplosionModel = Model::create(FLAG_RED|FLAG_GREEN|FLAG_BLUE|FLAG_ALPHA, FLAG_ALPHA|FLAG_ANGLE, FLAG_ANGLE);
     ExplosionModel->setParam(PARAM_ALPHA, 1.0f, 0.0f);
     ExplosionModel->setParam(PARAM_ANGLE, 0.f, 2.0*3.14f, 0.f, 4.0*3.14f);
     ExplosionModel->setLifeTime(5.0f, 5.0f);
     //ExplosionModel->setShared(true);

    IRR::IRRQuadRenderer* ExplosionRenderer = IRR::IRRQuadRenderer::create(oDev);
     ExplosionRenderer->setTexture(oDriver->getTexture("data/fx_smoke.png"));
	 ExplosionRenderer->setTexturingMode(TEXTURE_2D);
	 ExplosionRenderer->setScale(50.0f,50.0f);
     ExplosionRenderer->setBlending(BLENDING_ADD);
	 ExplosionRenderer->enableRenderingHint(DEPTH_WRITE,false);
	 //ExplosionRenderer->setShared(true);

    Group* ExplosionGroup = Group::create(ExplosionModel, 1000);
     ExplosionGroup->setRenderer(ExplosionRenderer);
     ExplosionGroup->addEmitter(ExplosionEmitter);
     ExplosionGroup->setFriction(3.0f);
     ExplosionGroup->setGravity(Vector3D(0.0f,-9.0f,0.0f));

    IRR::IRRSystem* Explosion = IRR::IRRSystem::create(oSM->getRootSceneNode(),oSM);
     Explosion->addGroup(ExplosionGroup);

    int explosionID = Explosion->getSPKID();//Explosion->SPK::Registerable::getID();//
*/

    cout<<"Objets : "<<SPKFactory::getInstance().getNbObjects()<<endl;

	//--------------------



    oTimer->start();

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
        bool bNoIRSrc = false;
        try
        {
        //==
            //On récupère les coordonnées du joueur
            //Cette methode peut throw
            Wiimote3d posPlayer = WMHdl.GetPlayerPos();


            //GetPlayerPos n'a pas throw : il y a assez de sources

            core::vector3df posCamera = Wiimote3dToWorld3d(posPlayer);


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
        //==
        }
        catch(int e)
        {
            if(e==EXC_WIIPOS_NOT_ENOUGH_IRSRC)
            {
                bNoIRSrc=true;
            }
        }


        /*==========================================================================================
        \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
        // Gestion du curseur droit /////////////////////////////////////////////////////////////////
        ==========================================================================================*/
        int nRightCursorException = 0;
        core::position2d<s32> posRightCur;
        try
        {
            //Recup de la position du curseur droit
            Wiimote2dPercent RightCurPercent = WMHdl.GetCursorPos(WMHDL_RIGHT);
            posRightCur.set(RightCurPercent.x*nScreenWidth,RightCurPercent.y*nScreenHeight);


            //Traitement des evenements
            Wiimote2dPercent EventCurPos;
            struct WiimoteCursorEvent Event = WMHdl.GetLastButtonEvent(WMHDL_RIGHT);
            if(Event.event!=EVENT_NONE)//Un evenement à traiter !
            {
                if(Event.button & (WIIMOTE_BUTTON_A | WIIMOTE_BUTTON_B))//Si le bouton A ou B est pressé
                {
                    //Recherche des collisions
                    core::line3d<f32> RightRay = oCollM->getRayFromScreenCoordinates(posRightCur, nodeCamera);

                    core::vector3df posHitPosition;
                    core::triangle3df triHitTriangle;
                    scene::ISceneNode *nodeSelected = oCollM->getSceneNodeAndCollisionPointFromRay(RightRay, posHitPosition, triHitTriangle, ID_TYPE_CIBLE,	0);
                    if(nodeSelected!=0)
                    {
                        nodeSelected->setVisible(false);

                        if(Event.button & WIIMOTE_BUTTON_A)
                        {

                        //GravEngine.FollowNode(nodeSelected, 1000, 10000);



                            //IRR::IRRSystem* newSystem = dynamic_cast<IRR::IRRSystem*>(SPKFactory::getInstance().copy(explosionID));
                            //IRR::IRRSystem* newSystem = SPK_Copy(IRR::IRRSystem,explosionID);
                            //newSystem->setPosition(posHitPosition);
                            //newSystem->setTransformPosition(Vector3D(posHitPosition.X, posHitPosition.Y, posHitPosition.Z));
                            //newSystem->updateTransform();
                            //partFire->setPosition(posHitPosition);
                            //partFire->setVisible(true);

    Point* ExplosionEmitZone = Point::create();

    RandomEmitter* ExplosionEmitter = RandomEmitter::create();
     ExplosionEmitter->setZone(ExplosionEmitZone);
     ExplosionEmitter->setTank(20);
     ExplosionEmitter->setFlow(100);
     ExplosionEmitter->setForce(150.0, 300.0);

    Model* ExplosionModel = Model::create(FLAG_RED|FLAG_GREEN|FLAG_BLUE|FLAG_ALPHA, FLAG_ALPHA|FLAG_ANGLE, FLAG_ANGLE);
     ExplosionModel->setParam(PARAM_ALPHA, 1.0f, 0.0f);
     ExplosionModel->setParam(PARAM_ANGLE, 0.f, 2.0*3.14f, 0.f, 4.0*3.14f);
     ExplosionModel->setLifeTime(0.5f, 2.0f);
     //ExplosionModel->setShared(true);

    IRR::IRRQuadRenderer* ExplosionRenderer = IRR::IRRQuadRenderer::create(oDev);
     ExplosionRenderer->setTexture(oDriver->getTexture("data/fx_smoke.png"));
	 ExplosionRenderer->setTexturingMode(TEXTURE_2D);
	 ExplosionRenderer->setScale(50.0f,100.0f);
     ExplosionRenderer->setBlending(BLENDING_ADD);
	 ExplosionRenderer->enableRenderingHint(DEPTH_WRITE,false);
	 //ExplosionRenderer->setShared(true);

    Group* ExplosionGroup = Group::create(ExplosionModel, 1000);
     ExplosionGroup->setRenderer(ExplosionRenderer);
     ExplosionGroup->addEmitter(ExplosionEmitter);
     ExplosionGroup->setFriction(2.5f);
     ExplosionGroup->setGravity(Vector3D(0.0f,-9.0f,0.0f));

    IRR::IRRSystem* Explosion = IRR::IRRSystem::create(oSM->getRootSceneNode(),oSM);
     Explosion->addGroup(ExplosionGroup);
     Explosion->setPosition(posHitPosition);

                        }
                        else if(Event.button & WIIMOTE_BUTTON_B)
                        {
                            partExplosion->setPosition(posHitPosition);
                            partExplosion->setVisible(true);
                        }
                    }
                }
                else if(Event.button & WIIMOTE_BUTTON_HOME)//Si le bouton home est pressé
                {
                    for(unsigned int i=0 ; i<nodeCibles.size() ; i++)
                        nodeCibles[i]->setVisible(true);
                }
                else
                {
                    partExplosion->setVisible(false);
                    partFire->setVisible(false);
                }

            }
        }
        catch(int e)
        {
            nRightCursorException=e;
        }


        /*====================================================================================================================
        \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
         >> ELEMENTS GUI ! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ====================================================================================================================*/
        //Rendre la scène
        oSM->drawAll();


        //Notification GUI src lunettes
        if(bNoIRSrc)
            oDriver->draw2DImage(texNoIRSrc, core::position2d<s32>(50,50),
                        core::rect<s32>(0,0,200,200), 0,
                        video::SColor(255, 255, 255, 255), true);


        //Dessin du curseur droit
        if(nRightCursorException==0)
            oDriver->draw2DImage(texRightCrosshair, core::position2d<s32>(posRightCur.X-32, posRightCur.Y-32),
                        core::rect<s32>(0,0,64,64), 0,
                        video::SColor(255, 255, 255, 255), true);



        //Affichage de la scène
        oDriver->endScene();



        //Frametime
        core::stringw sCaption = L"FPS=";
        sCaption += oDriver->getFPS();//oTimer->getTime()
        oDev->setWindowCaption(sCaption.c_str());
    }
}

