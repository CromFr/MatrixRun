#include "VfxManager.hpp"

using namespace std;
using namespace irr;
using namespace SPK;

namespace game
{
	vector<Registerable*> VfxManager::m_vfxList;

	/*====================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	====================================================================================================================*/
	void VfxManager::Init(IrrlichtDevice* oDev)
	{
		using namespace IRR;
		//video::IVideoDriver* oDriver = oDev->getVideoDriver();
		scene::ISceneManager *oSM = oDev->getSceneManager();
	//================================================================
	//vfx_bullet_shot
		{
		m_vfxList.push_back(0);
		}

	//================================================================
	//vfx_bullet_trail
		{
			Point* zone = Point::create();

			StaticEmitter* emit_trail = StaticEmitter::create();
			 emit_trail->setZone(zone);
			 emit_trail->setTank(-1);
			 emit_trail->setFlow(200);
			 //emit_trail->setShared(true);
			StaticEmitter* emit_smoke = StaticEmitter::create();
			 emit_smoke->setZone(zone);
			 emit_smoke->setTank(-1);
			 emit_smoke->setFlow(30);
			 //emit_smoke->setShared(true);

			Model* model_trail = Model::create(FLAG_ALPHA|FLAG_ANGLE|FLAG_TEXTURE_INDEX, FLAG_ALPHA|FLAG_TEXTURE_INDEX, FLAG_ANGLE);
			 model_trail->setParam(PARAM_ALPHA, 1.0, 0.0);
			 model_trail->setParam(PARAM_ANGLE, 0.f, 2.0*3.14);
			 model_trail->setParam(PARAM_TEXTURE_INDEX, 0, 32);
			 model_trail->setLifeTime(0.5, 0.7);
			 model_trail->setShared(true);
			Model* model_smoke = Model::create(FLAG_ALPHA|FLAG_ANGLE|FLAG_SIZE, FLAG_ALPHA|FLAG_SIZE, FLAG_ANGLE);
			 model_smoke->setParam(PARAM_ALPHA, 0.5, 0.0);
			 model_smoke->setParam(PARAM_SIZE, 0.0, 3.0);
			 model_smoke->setParam(PARAM_ANGLE, 0.f, 2.0*3.14);
			 model_smoke->setLifeTime(1, 2);
			 model_smoke->setShared(true);

			IRRQuadRenderer* render_trail = IRRQuadRenderer::create(oDev);
			 render_trail->setTexture(res::material::Get("bullet_trail_blue.png"));
			 render_trail->setTexturingMode(TEXTURE_2D);
			 render_trail->setAtlasDimensions(8,4);
			 render_trail->setScale(10.0,10.0);
			 render_trail->setBlending(BLENDING_ADD);
			 render_trail->enableRenderingHint(DEPTH_WRITE,false);
			 render_trail->setShared(true);
			IRRQuadRenderer* render_smoke = IRRQuadRenderer::create(oDev);
			 render_smoke->setTexture(res::material::Get("shockwave1.png"));
			 render_smoke->setTexturingMode(TEXTURE_2D);
			 render_smoke->setScale(10.0,10.0);
			 render_smoke->setBlending(BLENDING_ALPHA);
			 render_smoke->enableRenderingHint(DEPTH_WRITE,false);
			 render_smoke->setShared(true);

			Group* group_trail = Group::create(model_trail, 1000);
			 group_trail->setRenderer(render_trail);
			 group_trail->addEmitter(emit_trail);
			 //group_trail->setShared(true);
			Group* group_smoke = Group::create(model_smoke, 1000);
			 group_smoke->setRenderer(render_smoke);
			 group_smoke->addEmitter(emit_smoke);
			 group_smoke->setGravity(Vector3D(0.0f,5.0f,0.0f));
			 //group_smoke->setShared(true);

			IRRSystem* sys = IRRSystem::create(0,oSM, false);
			 sys->addGroup(group_trail);
			 sys->addGroup(group_smoke);
			 sys->setVisible(false);

			m_vfxList.push_back(sys);
		}
	//================================================================
	//vfx_bullet_impact
		{
			Point* zone = Point::create();

			StaticEmitter* emit_sw0 = StaticEmitter::create();
			 emit_sw0->setZone(zone);
			 emit_sw0->setTank(1);
			 emit_sw0->setFlow(-1);
			 //emit_sw0->setShared(true);
			StaticEmitter* emit_sw1 = StaticEmitter::create();
			 emit_sw1->setZone(zone);
			 emit_sw1->setTank(1);
			 emit_sw1->setFlow(-1);
			 //emit_sw1->setShared(true);
			RandomEmitter* emit_pt = RandomEmitter::create();
			 emit_pt->setZone(zone);
			 emit_pt->setTank(40);
			 emit_pt->setFlow(-1);
			 emit_pt->setForce(100, 150);
			 //emit_pt->setShared(true);

			Model* model_sw0 = Model::create(FLAG_ALPHA|FLAG_SIZE, FLAG_ALPHA|FLAG_SIZE);
			 model_sw0->setParam(PARAM_ALPHA, 1.0, 0.0);
			 model_sw0->setParam(PARAM_SIZE, 0, 10);
			 model_sw0->setLifeTime(0.6, 1);
			 model_sw0->setShared(true);
			Model* model_sw1 = Model::create(FLAG_ALPHA|FLAG_SIZE|FLAG_ANGLE, FLAG_ALPHA|FLAG_SIZE, FLAG_ANGLE);
			 model_sw1->setParam(PARAM_ALPHA, 1.0, 0.0);
			 model_sw1->setParam(PARAM_SIZE, 2,2.5);
			 model_sw1->setParam(PARAM_ANGLE, 0,2*3.14);
			 model_sw1->setLifeTime(0.4, 0.7);
			 model_sw1->setShared(true);
			Model* model_pt = Model::create(FLAG_RED|FLAG_GREEN|FLAG_ALPHA, FLAG_ALPHA);
			 model_pt->setParam(PARAM_ALPHA, 1.0, 0.0);
			 model_pt->setParam(PARAM_GREEN, 0.6);
			 model_pt->setParam(PARAM_RED, 0);
			 model_pt->setLifeTime(0.4, 0.5);
			 model_pt->setShared(true);

			IRRQuadRenderer* render_sw0 = IRRQuadRenderer::create(oDev);
			 render_sw0->setTexture(res::material::Get("shockwave0.png"));
			 render_sw0->setTexturingMode(TEXTURE_2D);
			 render_sw0->setScale(10.0,10.0);
			 render_sw0->setBlending(BLENDING_ADD);
			 render_sw0->enableRenderingHint(DEPTH_WRITE,false);
			 //render_sw0->setShared(true);
			IRRQuadRenderer* render_sw1 = IRRQuadRenderer::create(oDev);
			 render_sw1->setTexture(res::material::Get("shockwave1.png"));
			 render_sw1->setTexturingMode(TEXTURE_2D);
			 render_sw1->setScale(10.0,10.0);
			 render_sw1->setBlending(BLENDING_ADD);
			 render_sw1->enableRenderingHint(DEPTH_WRITE,false);
			 //render_sw1->setShared(true);
			IRRPointRenderer* render_pt = IRRPointRenderer::create(oDev);
			 render_pt->setSize(5.0);

			Group* group_sw0 = Group::create(model_sw0, 1);
			 group_sw0->setRenderer(render_sw0);
			 group_sw0->addEmitter(emit_sw0);
			 //group_sw0->setShared(true);
			Group* group_sw1 = Group::create(model_sw1, 1);
			 group_sw1->setRenderer(render_sw1);
			 group_sw1->addEmitter(emit_sw1);
			 //group_sw1->setShared(true);
			Group* group_pt = Group::create(model_pt, 40);
			 group_pt->setRenderer(render_pt);
			 group_pt->addEmitter(emit_pt);
			 group_pt->setFriction(3.0);
			 //group_sw1->setShared(true);


			IRRSystem* sys = IRRSystem::create(0,oSM, false);
			 sys->addGroup(group_sw0);
			 sys->addGroup(group_sw1);
			 sys->addGroup(group_pt);
			 sys->setVisible(false);

			m_vfxList.push_back(sys);
		}
	//================================================================
	//vfx_blood_sentinel
		{
			Point* zone = Point::create();

			RandomEmitter* emit_pt = RandomEmitter::create();
			 emit_pt->setZone(zone);
			 emit_pt->setTank(200);
			 emit_pt->setFlow(-1);
			 emit_pt->setForce(20, 80);

			Model* model_pt = Model::create(FLAG_RED|FLAG_GREEN|FLAG_ALPHA, FLAG_ALPHA|FLAG_GREEN);
			 model_pt->setParam(PARAM_ALPHA, 1.0, 0);
			 model_pt->setParam(PARAM_RED, 1);
			 model_pt->setParam(PARAM_GREEN, 1, 0.7);
			 //model_pt->setParam(PARAM_BLUE, 0.0, 0.0, 1.0, 1.0);
			 model_pt->setLifeTime(0.5, 1.0);
			 model_pt->setShared(true);

			IRRPointRenderer* render_pt = IRRPointRenderer::create(oDev);
			 render_pt->setSize(5.0);

			Group* group_pt = Group::create(model_pt, 200);
			 group_pt->setRenderer(render_pt);
			 group_pt->addEmitter(emit_pt);
			 group_pt->setFriction(2.0);
			 group_pt->setGravity(Vector3D(0,-60,0));

			IRRSystem* sys = IRRSystem::create(0,oSM, false);
			 sys->addGroup(group_pt);
			 sys->setVisible(false);

			m_vfxList.push_back(sys);
		}



	}
	/*====================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	====================================================================================================================*/


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
		for(unsigned int i=0 ; i<nGroupCount ; i++)
		{
			const vector<Emitter*> vEmitList(vGroupList[i]->getEmitters());
			unsigned int nEmitCount = vEmitList.size();
			for(unsigned int j=0 ; j<nEmitCount ; j++)
			{
				vEmitList[j]->setActive(false);
			}
		}

	}



}
