#ifndef COLLISIONNABLE_HPP_INCLUDED
#define COLLISIONNABLE_HPP_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "CollGeometry.hpp"


namespace game
{
	class MRCollisionnable : public scene::ISceneNode
	{
	public:
		MRCollisionnable(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
				const core::vector3df& position = core::vector3df(0,0,0))
			: ISceneNode(parent, mgr, id, position)
		{
			m_CollM = mgr->getSceneCollisionManager();
			m_nCollisionFlags = 0;
		}
		~MRCollisionnable()
		{
			ClearCollisions();
		}

		inline void SetCollisionFlags(int nFlags){m_nCollisionFlags = nFlags;}
		inline int GetCollisionFlags()const{return m_nCollisionFlags;}


		void AddCollisionLine(core::line3df& line)
		{
			m_CollPoints.push_back(new coll::CollLine(this, line));
		}
		void AddCollisionPoint(core::vector3df& point)
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

		virtual void OnCollision(scene::ISceneNode* node, core::triangle3df& triangle, core::vector3df& position)=0;


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
						OnCollision(collision->node, collision->triangle, collision->position);
					}
				}
			}
		}

		scene::ISceneCollisionManager* m_CollM;

		int m_nCollisionFlags;

	private:
		vector<coll::CollGeometry*> m_CollPoints;


	};






}

#endif // COLLISIONNABLE_HPP_INCLUDED
