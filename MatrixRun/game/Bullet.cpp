#include "Bullet.hpp"

using namespace std;
using namespace irr;
using namespace boost;


namespace game
{



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
	Bullet::Bullet(scene::ISceneNode* parent, scene::ISceneManager* mgr,
			const core::vector3df& position, const core::vector3df& direction, float fSpeedS)
		: IEmptySceneNode(parent, mgr, bullet, position)
	{
		//Collisions
		m_collisionnable = new Collisionnable(this, mgr, 0, bind(&Bullet::OnCollision, this, _1, _2, _3));
		m_collisionnable->SetCollisionFlags(tunnel_border|sentinel|ship);
		m_collisionnable->AddCollisionPoint(core::vector3df(0,0,0));

		//Dynamic light
		m_light = new scene::IDynamicLightSceneNode(this, mgr, 0, -direction*15, 30.0, video::SColor(128,0,144,255));

		//Animator
		m_animator = new scene::IConstantSpeedAnimator(direction, fSpeedS);
		addAnimator(m_animator);

		//Visual effects
		m_effect = VfxManager::AddVfxSceneNode(this, vfx_bullet_trail);
	}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
	void Bullet::OnCollision(const scene::ISceneNode* node, const core::triangle3df& triangle, const core::vector3df& position)
	{
		Object* oContainer = dynamic_cast<Object*>(node->getParent());
		enum ObjectType nType = oContainer->GetType();
		if(nType&sentinel)
		{
			Sentinel* oSentinel = dynamic_cast<Sentinel*>(oContainer);
			oSentinel->ApplyDamage(40);
		}
		else if(nType&ship)
		{
			//Apply damage to player

		}
		getSceneManager()->addToDeletionQueue(m_collisionnable);//Delete the collisionnable
		VfxManager::StopEmitting(m_effect);//Stop the trail effect
		removeAnimators();//Stop moving
		VfxManager::AddVfxSceneNode(this, vfx_bullet_impact);//impact effect

		//Light effect
		// TODO (crom#1#): Change parent of light to follow the percuted node
		m_light->FadeToColor(video::SColor(255,50,180,255), 0.2);
		Async::DelayCommand(0.2, bind(&scene::IDynamicLightSceneNode::FadeToColor, m_light, video::SColor(0,0,0,0), 1.8));

		//Remove the node
		Async::DelayCommand(2.5, bind(&scene::ISceneManager::addToDeletionQueue, getSceneManager(), m_collisionnable));

	}

}
