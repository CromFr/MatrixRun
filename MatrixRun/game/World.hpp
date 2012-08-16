#ifndef GAMEMANAGER_HPP_INCLUDED
#define GAMEMANAGER_HPP_INCLUDED

#include <irrlicht.h>
#include "../irr/IConstantSpeedAnimator.h"
#include "../irr/IEmptySceneNode.h"
using namespace irr;

#include <deque>
using namespace std;

#include "Tunnel.hpp"

namespace game
{
	class World : public scene::IEmptySceneNode
	{
	public:
		World(scene::ISceneManager* mgr)
			: IEmptySceneNode(mgr->getRootSceneNode(), mgr, -1, core::vector3df(0,0,0)),
			m_animator(core::vector3df(0,0,1), -400)
		{
			addAnimator(&m_animator);

			// @todo (crom#1#): Optimize
			m_dqTunnelBorders.push_back(new Tunnel(this, mgr, core::vector3df(0,0,-300), base::Elipse(core::vector2df(0,0), 140, 120, 0), base::Elipse(core::vector2df(0,0), 140, 120, 0)));
			for(int i=0 ; i<=2700 ; i+=300)
			{
				const base::Elipse* elipseLast = m_dqTunnelBorders.back()->GetEndingElipse();
				m_dqTunnelBorders.push_back(new Tunnel(this, mgr, core::vector3df(0,0,i), *elipseLast, RandomizeElipse(*elipseLast) ));

			}
		}
		~World()
		{
			for(unsigned int i=0 ; i<m_dqTunnelBorders.size() ; i++)
			{
				m_dqTunnelBorders[i]->remove();
				delete m_dqTunnelBorders[i];
			}
		}

		void OnAnimate(u32 timeMs)
		{
			//Generation of the tunnel
			float fZ = m_dqTunnelBorders.front()->getAbsolutePosition().Z;
			if(fZ<=-400)
			{
				//modulo position
				RelativeTranslation.Z+=300;//+300z for the container
				for(core::list<ISceneNode*>::Iterator it = Children.begin() ; it!=Children.end() ; it++)//-300 for the children
				{
					core::vector3df newpos = (*it)->getPosition();
					newpos.Z-=300;
					(*it)->setPosition(newpos);
				}

				m_dqTunnelBorders.pop_front();
				const base::Elipse* elipseLast = m_dqTunnelBorders.back()->GetEndingElipse();
				m_dqTunnelBorders.push_back(new Tunnel(this, getSceneManager(), core::vector3df(0,0,fZ+3000.0)-getAbsolutePosition(), *elipseLast, RandomizeElipse(*elipseLast)));
			}

			//Animate children
			IEmptySceneNode::OnAnimate(timeMs);
		}

	private:
		base::Elipse RandomizeElipse(const base::Elipse& base)
		{
			float fAngle = base.GetAngle() + (rand()%40)-20;

			core::vector2df vCenter(base.GetCenter().X+(rand()%50)-25, base.GetCenter().Y+(rand()%50)-25);
			if(vCenter.X>100)vCenter.X = 100;
			if(vCenter.X<-100)vCenter.X = -100;
			if(vCenter.Y>100)vCenter.Y = 100;
			if(vCenter.Y<-100)vCenter.Y = -100;

			float fA = base.GetA() + rand()%50 -25;
			if(fA<50)fA=50;
			if(fA>150)fA=150;
			float fB = base.GetB() + rand()%50 -25;
			if(fB<50)fB=50;
			if(fB>150)fB=150;

			return base::Elipse(vCenter, fA, fB, fAngle);
		}



		deque<Tunnel*> m_dqTunnelBorders;
		scene::IConstantSpeedAnimator m_animator;


	};
}


#endif // GAMEMANAGER_HPP_INCLUDED
