#include "Collisionnable.hpp"

#include "CollisionNode.hpp"

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace game
{
	Collisionnable::Collisionnable(irr::scene::ISceneNode* node, int nCollisionFlags)
	{
		m_collNode = new CollisionNode(node, node->getSceneManager(), 0, boost::bind(&Collisionnable::OnCollision, this, _1, _2, _3), nCollisionFlags);
	}

	Collisionnable::~Collisionnable()
	{
		delete m_collNode;
	}
}
