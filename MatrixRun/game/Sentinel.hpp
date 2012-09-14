#ifndef SENTINEL_HPP_INCLUDED
#define SENTINEL_HPP_INCLUDED

#include "Destroyable.hpp"
#include "../irr/IEmptySceneNode.h"

#include "VfxManager.hpp"

namespace game
{
	/**
	@brief Beware of the naughty sentinels !
	**/
	class Sentinel : public Destroyable, public irr::scene::IEmptySceneNode
	{
	public:
		Sentinel(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr,
				const irr::core::vector3df& position);



	private:
		virtual void OnDamaged(int nAmount)
		{
			std::cout<<"OnDamaged"<<std::endl;
			VfxManager::AddVfxSceneNode(this, VfxManager::blood_sentinel);
		}
		virtual void OnDeath()
		{

		}

		void OnCollision(const irr::scene::ISceneNode* node, const irr::core::triangle3df& triangle, const irr::core::vector3df& position)
		{

		}



		enum ObjectType GetType(){return sentinel;}

		irr::scene::IMeshSceneNode* m_model;
		irr::scene::ILightSceneNode* m_light;
		//Collisionnable m_collisionnable;
	};



}


#endif // SENTINEL_HPP_INCLUDED
