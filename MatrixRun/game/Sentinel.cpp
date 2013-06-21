#include "Sentinel.hpp"

using namespace irr;
using namespace std;
//using namespace boost;

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
		  //m_collisionnable(this, this->getSceneManager(), 0, bind(&Sentinel::OnCollision, this, _1, _2, _3)),
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
//		m_collisionnable.SetCollisionFlags(tunnel_border|tunnel_block|sentinel|ship);
//		m_collisionnable.AddCollisionPoint(core::vector3df(0,0,0));


		addAnimator(&m_animator);

	}
}
