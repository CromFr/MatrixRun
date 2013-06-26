#ifndef COLLISIONNABLE_HPP
#define COLLISIONNABLE_HPP

#include <irrlicht.h>
#include "CollisionNode.hpp"

namespace game
{

	/**
	@brief Interface for collisionnable objects, generating OnCollision events
	@note Must be inherited to construct
	**/
	class Collisionnable
	{
	protected:
		Collisionnable(irr::scene::ISceneNode* parent, int nCollisionFlags=0);
		~Collisionnable();

		virtual void OnCollision(irr::scene::ISceneNode* node, const irr::core::triangle3df& triangle, const irr::core::vector3df& position) = 0;

		CollisionNode* GetCollisionNode(){return m_collNode;}

	private:
		CollisionNode* m_collNode;

	};
}

#endif // COLLISIONNABLE_HPP
