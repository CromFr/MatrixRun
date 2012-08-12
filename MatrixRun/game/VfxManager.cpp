#include "VfxManager.hpp"

namespace game
{
	vector<Registerable*> VfxManager::m_vfxList;

	void VfxManager::Init(IrrlichtDevice* oDev)
	{
		using namespace IRR;
		video::IVideoDriver* oDriver = oDev->getVideoDriver();
		scene::ISceneManager *oSM = oDev->getSceneManager();
	//================================================================
	//vfx_bullet_shot
		{
		m_vfxList.push_back(0);
		}

	//================================================================
	//vfx_bullet_trail
		{
			Point* zoneBulletTrail = Point::create();

			StaticEmitter* emitBulletTrail = StaticEmitter::create();
			 emitBulletTrail->setZone(zoneBulletTrail);
			 emitBulletTrail->setTank(-1);
			 emitBulletTrail->setFlow(200);
			 //emitBulletTrail->setShared(true);
			StaticEmitter* emitBulletTrail_smoke = StaticEmitter::create();
			 emitBulletTrail_smoke->setZone(zoneBulletTrail);
			 emitBulletTrail_smoke->setTank(-1);
			 emitBulletTrail_smoke->setFlow(30);
			 //emitBulletTrail_smoke->setShared(true);

			Model* modelBulletTrail = Model::create(FLAG_ALPHA|FLAG_ANGLE|FLAG_TEXTURE_INDEX, FLAG_ALPHA|FLAG_TEXTURE_INDEX, FLAG_ANGLE);
			 modelBulletTrail->setParam(PARAM_ALPHA, 1.0, 0.0);
			 modelBulletTrail->setParam(PARAM_ANGLE, 0.f, 2.0*3.14);
			 modelBulletTrail->setParam(PARAM_TEXTURE_INDEX, 0, 32);
			 modelBulletTrail->setLifeTime(0.5, 0.7);
			 modelBulletTrail->setShared(true);
			Model* modelBulletTrail_smoke = Model::create(FLAG_ALPHA|FLAG_ANGLE|FLAG_SIZE, FLAG_ALPHA|FLAG_SIZE, FLAG_ANGLE);
			 modelBulletTrail_smoke->setParam(PARAM_ALPHA, 0.5, 0.0);
			 modelBulletTrail_smoke->setParam(PARAM_SIZE, 0.0, 3.0);
			 modelBulletTrail_smoke->setParam(PARAM_ANGLE, 0.f, 2.0*3.14);
			 modelBulletTrail_smoke->setLifeTime(1, 2);
			 modelBulletTrail_smoke->setShared(true);

			IRRQuadRenderer* renderBulletTrail = IRRQuadRenderer::create(oDev);
			 renderBulletTrail->setTexture(res::material::Get("bullet_trail_blue.png"));
			 renderBulletTrail->setTexturingMode(TEXTURE_2D);
			 renderBulletTrail->setAtlasDimensions(8,4);
			 renderBulletTrail->setScale(10.0,10.0);
			 renderBulletTrail->setBlending(BLENDING_ADD);
			 renderBulletTrail->enableRenderingHint(DEPTH_WRITE,false);
			 renderBulletTrail->setShared(true);
			IRRQuadRenderer* renderBulletTrail_smoke = IRRQuadRenderer::create(oDev);
			 renderBulletTrail_smoke->setTexture(res::material::Get("shockwave1.png"));
			 renderBulletTrail_smoke->setTexturingMode(TEXTURE_2D);
			 renderBulletTrail_smoke->setScale(10.0,10.0);
			 renderBulletTrail_smoke->setBlending(BLENDING_ALPHA);
			 renderBulletTrail_smoke->enableRenderingHint(DEPTH_WRITE,false);
			 renderBulletTrail_smoke->setShared(true);

			Group* groupBulletTrail = Group::create(modelBulletTrail, 1000);
			 groupBulletTrail->setRenderer(renderBulletTrail);
			 groupBulletTrail->addEmitter(emitBulletTrail);
			 groupBulletTrail->setShared(true);
			Group* groupBulletTrail_smoke = Group::create(modelBulletTrail_smoke, 1000);
			 groupBulletTrail_smoke->setRenderer(renderBulletTrail_smoke);
			 groupBulletTrail_smoke->addEmitter(emitBulletTrail_smoke);
			 groupBulletTrail_smoke->setGravity(Vector3D(0.0f,5.0f,0.0f));
			 groupBulletTrail_smoke->setShared(true);

			IRRSystem* sysBulletTrail = IRRSystem::create(0,oSM);
			 sysBulletTrail->addGroup(groupBulletTrail);
			 sysBulletTrail->addGroup(groupBulletTrail_smoke);
			 sysBulletTrail->setVisible(false);

			m_vfxList.push_back(sysBulletTrail);
		}
	//================================================================
	//vfx_bullet_impact
		{
			Point* zoneBulletImpact = Point::create();

			StaticEmitter* emitBulletImpact_sw0 = StaticEmitter::create();
			 emitBulletImpact_sw0->setZone(zoneBulletImpact);
			 emitBulletImpact_sw0->setTank(1);
			 emitBulletImpact_sw0->setFlow(-1);
			 //emitBulletImpact_sw0->setShared(true);
			StaticEmitter* emitBulletImpact_sw1 = StaticEmitter::create();
			 emitBulletImpact_sw1->setZone(zoneBulletImpact);
			 emitBulletImpact_sw1->setTank(1);
			 emitBulletImpact_sw1->setFlow(-1);
			 //emitBulletImpact_sw1->setShared(true);
			RandomEmitter* emitBulletImpact_pt = RandomEmitter::create();
			 emitBulletImpact_pt->setZone(zoneBulletImpact);
			 emitBulletImpact_pt->setTank(40);
			 emitBulletImpact_pt->setFlow(-1);
			 emitBulletImpact_pt->setForce(100, 150);
			 //emitBulletImpact_pt->setShared(true);

			Model* modelBulletImpact_sw0 = Model::create(FLAG_ALPHA|FLAG_SIZE, FLAG_ALPHA|FLAG_SIZE);
			 modelBulletImpact_sw0->setParam(PARAM_ALPHA, 1.0, 0.0);
			 modelBulletImpact_sw0->setParam(PARAM_SIZE, 0, 10);
			 modelBulletImpact_sw0->setLifeTime(0.6, 1);
			 modelBulletImpact_sw0->setShared(true);
			Model* modelBulletImpact_sw1 = Model::create(FLAG_ALPHA|FLAG_SIZE|FLAG_ANGLE, FLAG_ALPHA|FLAG_SIZE, FLAG_ANGLE);
			 modelBulletImpact_sw1->setParam(PARAM_ALPHA, 1.0, 0.0);
			 modelBulletImpact_sw1->setParam(PARAM_SIZE, 2,2.5);
			 modelBulletImpact_sw1->setParam(PARAM_ANGLE, 0,2*3.14);
			 modelBulletImpact_sw1->setLifeTime(0.4, 0.7);
			 modelBulletImpact_sw1->setShared(true);
			Model* modelBulletImpact_pt = Model::create(FLAG_RED|FLAG_GREEN|FLAG_ALPHA, FLAG_ALPHA);
			 modelBulletImpact_pt->setParam(PARAM_ALPHA, 1.0, 0.0);
			 modelBulletImpact_pt->setParam(PARAM_GREEN, 0.6);
			 modelBulletImpact_pt->setParam(PARAM_RED, 0);
			 modelBulletImpact_pt->setLifeTime(0.4, 0.5);
			 modelBulletImpact_pt->setShared(true);

			IRRQuadRenderer* renderBulletImpact_sw0 = IRRQuadRenderer::create(oDev);
			 renderBulletImpact_sw0->setTexture(res::material::Get("shockwave0.png"));
			 renderBulletImpact_sw0->setTexturingMode(TEXTURE_2D);
			 renderBulletImpact_sw0->setScale(10.0,10.0);
			 renderBulletImpact_sw0->setBlending(BLENDING_ADD);
			 renderBulletImpact_sw0->enableRenderingHint(DEPTH_WRITE,false);
			 //renderBulletImpact_sw0->setShared(true);
			IRRQuadRenderer* renderBulletImpact_sw1 = IRRQuadRenderer::create(oDev);
			 renderBulletImpact_sw1->setTexture(res::material::Get("shockwave1.png"));
			 renderBulletImpact_sw1->setTexturingMode(TEXTURE_2D);
			 renderBulletImpact_sw1->setScale(10.0,10.0);
			 renderBulletImpact_sw1->setBlending(BLENDING_ADD);
			 renderBulletImpact_sw1->enableRenderingHint(DEPTH_WRITE,false);
			 //renderBulletImpact_sw1->setShared(true);
			IRRPointRenderer* renderBulletImpact_pt = IRRPointRenderer::create(oDev);
			 renderBulletImpact_pt->setSize(5.0);

			Group* groupBulletImpact_sw0 = Group::create(modelBulletImpact_sw0, 1);
			 groupBulletImpact_sw0->setRenderer(renderBulletImpact_sw0);
			 groupBulletImpact_sw0->addEmitter(emitBulletImpact_sw0);
			 //groupBulletImpact_sw0->setShared(true);
			Group* groupBulletImpact_sw1 = Group::create(modelBulletImpact_sw1, 1);
			 groupBulletImpact_sw1->setRenderer(renderBulletImpact_sw1);
			 groupBulletImpact_sw1->addEmitter(emitBulletImpact_sw1);
			 //groupBulletImpact_sw1->setShared(true);
			Group* groupBulletImpact_pt = Group::create(modelBulletImpact_pt, 40);
			 groupBulletImpact_pt->setRenderer(renderBulletImpact_pt);
			 groupBulletImpact_pt->addEmitter(emitBulletImpact_pt);
			 groupBulletImpact_pt->setFriction(3.0);
			 //groupBulletImpact_sw1->setShared(true);


			IRRSystem* sysBulletImpact = IRRSystem::create(0,oSM);
			 sysBulletImpact->addGroup(groupBulletImpact_sw0);
			 sysBulletImpact->addGroup(groupBulletImpact_sw1);
			 sysBulletImpact->addGroup(groupBulletImpact_pt);
			 sysBulletImpact->setVisible(false);

			m_vfxList.push_back(sysBulletImpact);
		}

	}


	IRR::IRRSystem* VfxManager::AddVfxSceneNode(scene::ISceneNode* parent, enum vfx effect, const core::vector3df& position)
	{
		IRR::IRRSystem* vfxNode = dynamic_cast<IRR::IRRSystem*>((SPKFactory::getInstance()).copy(m_vfxList[effect]));
		vfxNode->setParent(parent);
		vfxNode->setPosition(position);
		vfxNode->setVisible(true);
		return vfxNode;
	}

	void VfxManager::StopEmitting(IRR::IRRSystem* sys)
	{
		const vector<Group*> vGroupList(sys->getGroups());
		unsigned int nGroupCount = vGroupList.size();
		for(int i=0 ; i<nGroupCount ; i++)
		{
			const vector<Emitter*> vEmitList(vGroupList[i]->getEmitters());
			unsigned int nEmitCount = vEmitList.size();
			for(int j=0 ; j<nEmitCount ; j++)
			{
				vEmitList[j]->setActive(false);
			}
		}

	}



}
