//====================================
scene::IParticleEmitter* Emitter;
scene::IParticleAffector* Affector;
//====================================


//=========================================================================================================================================================================
scene::IParticleSystemSceneNode* partFire = oSM->addParticleSystemSceneNode(false, 0, -1, core::vector3df(0,0,50));
Emitter = partFire->createBoxEmitter(
    core::aabbox3d<f32>(-3,0,-3,3,1,3),     // coordonnees de la boite d'emission
    core::vector3df(0.0f,0.06f,0.0f),        // direction de diffusion
    80,100,                                       // nb particules emises a la sec min / max
    video::SColor(0,0,0,0),                  // couleur la plus sombre
    video::SColor(0,255,255,255),            // couleur la plus claire
    400, 800,                                    // duree de vie min / max
    10,                                            // angle max d'ecart / direction prevue
    core::dimension2df(10.0f,10.0f),           // taille minimum
    core::dimension2df(16.0f,16.0f));          // taille maximum
partFire->setEmitter(Emitter);
  Emitter->drop();
Affector = partFire->createFadeOutParticleAffector(video::SColor(0,0,0,0), 800);
partFire->addAffector(Affector);       // ajout du modificateur au particle system
  Affector->drop();
partFire->setMaterialFlag(video::EMF_LIGHTING, false);          // insensible a la lumiere
partFire->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);     // desactive zbuffer pour surfaces derriere
partFire->setMaterialTexture(0, oDriver->getTexture("data/vfx/fx_fire1.tga"));     // on colle une texture
partFire->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // application transparence
partFire->setVisible(false);
//=========================================================================================================================================================================
scene::IParticleSystemSceneNode* partExplosion = oSM->addParticleSystemSceneNode(false, 0, -1, core::vector3df(0,0,50));
Emitter = partExplosion->createBoxEmitter(
    core::aabbox3d<f32>(-15,-15,-1,15,15,1),     // coordonnees de la boite d'emission
    core::vector3df(0.0f,0.06f,0.0f),        // direction de diffusion
    15,30,                                       // nb particules emises a la sec min / max
    video::SColor(0xAA9155),                  // couleur la plus sombre
    video::SColor(0x9B8964),            // couleur la plus claire
    200, 400,                                    // duree de vie min / max
    360,                                            // angle max d'ecart / direction prevue
    core::dimension2df(50.0f,50.0f),           // taille minimum
    core::dimension2df(100.0f,100.0f));          // taille maximum
partExplosion->setEmitter(Emitter);
  Emitter->drop();
Affector = partExplosion->createFadeOutParticleAffector(video::SColor(0,0,255,0), 400);
partExplosion->addAffector(Affector);       // ajout du modificateur au particle system
  Affector->drop();
partExplosion->setMaterialFlag(video::EMF_LIGHTING, false);          // insensible a la lumiere
partExplosion->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);     // desactive zbuffer pour surfaces derriere
partExplosion->setMaterialTexture(0, oDriver->getTexture("data/vfx/dustgrad1.png"));     // on colle une texture
partExplosion->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // application transparence
partExplosion->setVisible(false);
//=========================================================================================================================================================================






/*
// Inits Particle Engine
randomSeed = oDev->getTimer()->getRealTime();
// Sets the update step
System::setClampStep(true,0.1f);			// clamp the step to 100 ms
System::useAdaptiveStep(0.001f,0.01f);		// use an adaptive step from 1ms to 10ms (1000fps to 100fps)

Vector3D gravity(0.0f,-0.8f,0.0f);

IRR::IRRQuadRenderer* particleRenderer = IRR::IRRQuadRenderer::create(oDev);
particleRenderer->setTexture(oDriver->getTexture("point.bmp"));
particleRenderer->setTexturingMode(TEXTURE_2D);
particleRenderer->setScale(0.05f,0.05f);
particleRenderer->setBlending(BLENDING_ADD);
particleRenderer->enableRenderingHint(DEPTH_WRITE,false);

// Model
Model* particleModel = Model::create(FLAG_RED | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA);
particleModel->setParam(PARAM_ALPHA,0.8f); // constant alpha
particleModel->setLifeTime(8.0f,8.0f);

// Emitter
SphericEmitter* particleEmitter = SphericEmitter::create(Vector3D(0.0f,1.0f,0.0f),0.1f * 3.14159365,0.1f * 3.14159365);
particleEmitter->setZone(Point::create(Vector3D(0.0f,0.02f,0.0f)));
particleEmitter->setFlow(250);
particleEmitter->setForce(1.5f,1.5f);

// Obstacle
Plane* groundPlane = Plane::create();
Obstacle* obstacle = Obstacle::create(groundPlane,INTERSECT_ZONE,0.6f,1.0f);

// Group
Group* particleGroup = Group::create(particleModel,2100);
particleGroup->addEmitter(particleEmitter);
particleGroup->setRenderer(particleRenderer);
particleGroup->addModifier(obstacle);
particleGroup->setGravity(gravity);
particleGroup->enableAABBComputing(true);

System* particleSystem = IRR::IRRSystem::create(oSM->getRootSceneNode(),oSM);
particleSystem->addGroup(particleGroup);
particleSystem->enableAABBComputing(true);
*/













