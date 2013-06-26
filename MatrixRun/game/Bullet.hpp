#ifndef BULLET_HPP_INCLUDED
#define BULLET_HPP_INCLUDED

#include "Object.hpp"
#include "../irr/IEmptySceneNode.h"
#include "Collisionnable.hpp"

#include "../irr/IConstantSpeedAnimator.h"
#include "../irr/IDynamicLightSceneNode.h"

namespace SPK{namespace IRR{class IRRSystem;}}

namespace game
{
	class Sentinel;

	/**
	@brief A bullet fired by the player that can cause damage to enemies
	**/
	class Bullet : public Object, public irr::scene::IEmptySceneNode, private Collisionnable
	{
	public:
		Bullet(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr,
				const irr::core::vector3df& position, const irr::core::vector3df& direction, float fSpeedS);

		virtual enum ObjectType GetType()const override {return bullet;}

	private:
		void OnCollision(irr::scene::ISceneNode* node, const irr::core::triangle3df& triangle, const irr::core::vector3df& position);

		SPK::IRR::IRRSystem* m_effect;
		irr::scene::IConstantSpeedAnimator* m_animator;
		irr::scene::IDynamicLightSceneNode* m_light;

	};
}

#endif // BULLET_HPP_INCLUDED
