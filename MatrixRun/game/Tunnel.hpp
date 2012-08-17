#ifndef TUNNEL_HPP_INCLUDED
#define TUNNEL_HPP_INCLUDED

#include <deque>
using namespace std;

#include "Object.hpp"
#include "../irr/IEmptySceneNode.h"

#include "../base/Elipse.hpp"

#include "TunnelModule.hpp"


namespace game
{
	///Scene node that contain multiple tunnel modules. The tunnel modules are auto-generated
	class Tunnel : public Object, public scene::IEmptySceneNode
	{
	public:
		///
		Tunnel(scene::ISceneNode* parent, scene::ISceneManager* mgr)
			: IEmptySceneNode(parent, mgr, tunnel_border)
		{
			// @todo (crom#1#): Optimize
			m_dqTunnelModules.push_back(new TunnelModule(this, mgr, core::vector3df(0,0,-TUNNEL_MODULE_DIM_Z), base::Elipse(core::vector2df(0,0), 140, 120, 0), base::Elipse(core::vector2df(0,0), 140, 120, 0)));
			for(int i=0 ; i<=9*TUNNEL_MODULE_DIM_Z ; i+=TUNNEL_MODULE_DIM_Z)
			{
				const base::Elipse* elipseLast = m_dqTunnelModules.back()->GetEndingElipse();
				TunnelModule* tunnel = new TunnelModule(this, mgr, core::vector3df(0,0,i), *elipseLast, RandomizeElipse(*elipseLast) );
				m_dqTunnelModules.push_back(tunnel);
			}
		}

		///Removes all dyn alloc (tunnel modules)
		~Tunnel()
		{
			for(unsigned int i=0 ; i<m_dqTunnelModules.size() ; i++)
			{
				m_dqTunnelModules[i]->remove();
				delete m_dqTunnelModules[i];
			}
		}


		void OnAnimate(u32 timeMs)
		{
			IEmptySceneNode::OnAnimate(timeMs);
			//Generation of the tunnel
			float fZ = m_dqTunnelModules.front()->getAbsolutePosition().Z;
			if(fZ<=-400)
			{
				//modulo position
				RelativeTranslation.Z+=TUNNEL_MODULE_DIM_Z;//+300z for the container
				for(core::list<ISceneNode*>::Iterator it = Children.begin() ; it!=Children.end() ; it++)//-300 for the children
				{
					core::vector3df newpos = (*it)->getPosition();
					newpos.Z-=TUNNEL_MODULE_DIM_Z;
					(*it)->setPosition(newpos);
				}

				m_dqTunnelModules.pop_front();
				const base::Elipse* elipseLast = m_dqTunnelModules.back()->GetEndingElipse();
				m_dqTunnelModules.push_back(new TunnelModule(this, getSceneManager(), core::vector3df(0,0,fZ+TUNNEL_MODULE_DIM_Z*10)-getAbsolutePosition(), *elipseLast, RandomizeElipse(*elipseLast)));
			}



		}

		//===========================================================================
		bool GetIsInTunnel(const core::vector3df& posAbs, scene::ISceneNode* outCheckedTunnel)
		{
			core::vector3df posCheckRelWorld = posAbs - getAbsolutePosition();

			int nPos = posCheckRelWorld.Z / TUNNEL_MODULE_DIM_Z;

			TunnelModule* tunnel = m_dqTunnelModules[nPos+1];
			core::vector3df postunnelRelWorld = tunnel->getPosition();
			if(posAbs.Z<postunnelRelWorld.Z+TUNNEL_MODULE_DIM_Z)
			{
				tunnel = m_dqTunnelModules[nPos+2];
				postunnelRelWorld.set(tunnel->getPosition());
			}

			outCheckedTunnel = tunnel;
			return tunnel->GetIsInTunnel(postunnelRelWorld-posCheckRelWorld);
		}


		///Implementation of Object
		virtual enum ObjectType GetType(){return tunnel_border;}

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


		deque<TunnelModule*> m_dqTunnelModules;



	};

}


#endif // TUNNEL_HPP_INCLUDED
