#include "Collisionnable.hpp"

using namespace irr;
using namespace boost;

namespace game
{

	/*====================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	====================================================================================================================*/
	Collisionnable::Collisionnable(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
			function<void(scene::ISceneNode*, core::triangle3df&, core::vector3df&)> funcCallback, int nCollisionFlags)
		: IEmptySceneNode(parent, mgr, id, core::vector3df(0,0,0))
	{
		m_CollM = mgr->getSceneCollisionManager();
		m_nCollisionFlags = nCollisionFlags;
		m_funcCallback = funcCallback;
	}


	/*====================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	====================================================================================================================*/
	Collisionnable::~Collisionnable()
	{
		ClearCollisions();
	}


	/*====================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	====================================================================================================================*/
	void Collisionnable::ClearCollisions()
	{
		unsigned int nSize = m_CollPoints.size();
		for(unsigned int i=0 ; i<nSize ; i++)
		{
			delete m_CollPoints[i];
		}
		m_CollPoints.clear();
	}


	/*====================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	====================================================================================================================*/
	void Collisionnable::OnAnimate(u32 timeMs)
	{
		ISceneNode::OnAnimate(timeMs);
		if(m_nCollisionFlags != 0)
		{
			unsigned int nSize = m_CollPoints.size();
			for(unsigned int i=0 ; i<nSize ; i++)
			{
				coll::Collision* collision = m_CollPoints[i]->GetCollision(m_CollM, this, m_nCollisionFlags);
				if(collision>0)
				{
					m_funcCallback(collision->node, collision->triangle, collision->position);
					break;
				}
			}
		}
	}






}