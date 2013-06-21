#ifndef SENTINEL_HPP_INCLUDED
#define SENTINEL_HPP_INCLUDED

#include "Object.hpp"
#include "Destroyable.hpp"
#include "../irr/IEmptySceneNode.h"

#include "../irr/IGravityAnimator.h"
#include "VfxManager.hpp"

namespace game
{
	class Collisionnable;

	/**
	@brief Beware of the naughty sentinels !
	**/
	class Sentinel : public Object, public Destroyable, public irr::scene::IEmptySceneNode
	{
	public:
		Sentinel(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, const irr::core::vector3df& position);
		~Sentinel();


	private:
		virtual void OnDamaged(int nAmount)
		{
			VfxManager::AddVfxSceneNode(this, VfxManager::blood_sentinel);
		}
		virtual void OnDeath()
		{
			m_animator.SetGravity(98);
		}

		void OnCollision(const irr::scene::ISceneNode* node, const irr::core::triangle3df& triangle, const irr::core::vector3df& position)
		{
			//addToDeletionQueue(this);

		}



		enum ObjectType GetType()const {return sentinel;}

		irr::scene::IMeshSceneNode* m_model;
		irr::scene::ILightSceneNode* m_light;
		Collisionnable* m_collisionnable;

		irr::scene::IGravityAnimator m_animator;
	};



}


#endif // SENTINEL_HPP_INCLUDED
