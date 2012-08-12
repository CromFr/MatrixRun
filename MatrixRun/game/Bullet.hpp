#ifndef BULLET_HPP_INCLUDED
#define BULLET_HPP_INCLUDED

#include "Object.hpp"
#include "Collisionnable.hpp"
#include "VfxManager.hpp"

#include "../irr/IConstantSpeedAnimator.h"
#include "../irr/IDynamicLightSceneNode.h"

#include "../lib/Async.hpp"


namespace game
{

	class Bullet : public Object
	{
	public:
		Bullet(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
				const core::vector3df& position, const core::vector3df& direction, float fSpeedS)
			: Object(parent, mgr, id, position),
			  //Animator
			  m_animator(direction, fSpeedS),
			  //Dynamic light
			  m_light(this, this->getSceneManager(), 0, core::vector3df(0,0,-10), 30.0, video::SColor(128,0,144,255)),
			  //Collisions
			  m_collisionnable(this, this->getSceneManager(), 0, bind(&Bullet::OnCollision, this, _1, _2, _3))
		{
			//Collisions
			m_collisionnable.SetCollisionFlags(border|block|sentinel|ship);
			m_collisionnable.AddCollisionPoint(core::vector3df(0,0,0));

			//Animator
			addAnimator(&m_animator);

			//Visual effects
			m_effect = VfxManager::AddVfxSceneNode(this, vfx_bullet_trail);

		}

		virtual enum ObjectType GetType(){return bullet;}

	private:
		void OnCollision(const scene::ISceneNode* node, const core::triangle3df& triangle, const core::vector3df& position)
		{
// TODO (crom#1#): Décommenter quand j'aurais codé d'autres objects. (le parent du node collisionné doit être un object !)
			//Object* oContainer = dynamic_cast<Object*>(node->getParent());

			enum ObjectType nType = sentinel;//oContainer->GetType();
			if(nType&sentinel)
			{
				//Apply damage to sentinel

				//remove();
			}
			else if(nType&ship)
			{
				//Apply damage to player

				//remove();
			}
			else if(nType&(border|block))
			{
				//remove();
			}
			VfxManager::StopEmitting(m_effect);//Stop the trail
			removeAnimators();//Stop moving
			VfxManager::AddVfxSceneNode(this, vfx_bullet_impact);//impact effect

			//Light effect
			m_light.FadeToColor(video::SColor(255,50,180,255), 0.2);
			Async::DelayCommand(0.2, bind(&scene::IDynamicLightSceneNode::FadeToColor, &m_light, video::SColor(0,0,0,0), 1.8));

			//Remove the node
			Async::DelayCommand(2.5, bind(&ISceneNode::remove, this));

		}

		SPK::IRR::IRRSystem* m_effect;
		scene::IConstantSpeedAnimator m_animator;
		scene::IDynamicLightSceneNode m_light;
		Collisionnable m_collisionnable;

	};
}

#endif // BULLET_HPP_INCLUDED
