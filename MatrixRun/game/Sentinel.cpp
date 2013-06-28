#include "Sentinel.hpp"

using namespace irr;
using namespace std;

namespace game
{


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
	Sentinel::Sentinel(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr,
			const irr::core::vector3df& position)
		: Destroyable(100),
		  IEmptySceneNode(parent, mgr, sentinel, position),
		  Collisionnable(this),
		  m_animator(200, 0.0, 0.0, core::vector3df(0,0,100))
	{
		//3d model
		m_model = mgr->addMeshSceneNode(res::model::Get("sentinel.3ds"), this);
		m_model->setMaterialFlag(video::EMF_LIGHTING, true);
		m_model->setMaterialFlag(video::EMF_FOG_ENABLE, true);
		m_model->setTriangleSelector(mgr->createTriangleSelector(res::model::Get("sentinel.3ds"), m_model));

		//Light
		m_light = mgr->addLightSceneNode(this, core::vector3df(0,0,2), video::SColor(255,255,0,0), 5);

		//Collisions
		GetCollisionNode()->SetCollisionFlags(tunnel_border|tunnel_block|ship);
		GetCollisionNode()->AddCollisionPoint(core::vector3df(0,0,0));


		addAnimator(&m_animator);
	}


	Sentinel::~Sentinel()
	{
	}
}
