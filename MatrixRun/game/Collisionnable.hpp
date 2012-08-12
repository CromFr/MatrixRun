#ifndef COLLISIONNABLE_HPP_INCLUDED
#define COLLISIONNABLE_HPP_INCLUDED

#include "../irr/IEmptySceneNode.h"
using namespace irr;

#include "CollGeometry.hpp"

#include <boost/bind.hpp>
#include <boost/function.hpp>
using namespace boost;

namespace game
{
	class Collisionnable : public scene::IEmptySceneNode
	{
	public:
		Collisionnable(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
				function<void(scene::ISceneNode*, core::triangle3df&, core::vector3df&)> funcCallback)
			: IEmptySceneNode(parent, mgr, id, core::vector3df(0,0,0))
		{
			m_CollM = mgr->getSceneCollisionManager();
			m_nCollisionFlags = 0;
			m_funcCallback = funcCallback;
		}
		~Collisionnable()
		{
			ClearCollisions();
		}

		inline void SetCollisionFlags(int nFlags){m_nCollisionFlags = nFlags;}
		inline int GetCollisionFlags()const{return m_nCollisionFlags;}


		void AddCollisionLine(const core::line3df& line)
		{
			m_CollPoints.push_back(new coll::CollLine(this, line));
		}
		void AddCollisionPoint(const core::vector3df& point)
		{
			m_CollPoints.push_back(new coll::CollPoint(this, point));
		}
		void ClearCollisions()
		{
			unsigned int nSize = m_CollPoints.size();
			for(unsigned int i=0 ; i<nSize ; i++)
			{
				delete m_CollPoints[i];
			}
			m_CollPoints.clear();
		}



	protected:
		void OnAnimate(u32 timeMs)
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


		scene::ISceneCollisionManager* m_CollM;

		int m_nCollisionFlags;

	private:
		vector<coll::CollGeometry*> m_CollPoints;
		function<void(scene::ISceneNode*, core::triangle3df&, core::vector3df&)> m_funcCallback;


	};






}

#endif // COLLISIONNABLE_HPP_INCLUDED
