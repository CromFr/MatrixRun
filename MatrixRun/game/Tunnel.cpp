#include "Tunnel.hpp"

using namespace irr;
using namespace std;

namespace game
{


	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	Tunnel::Tunnel(scene::ISceneNode* parent, scene::ISceneManager* mgr)
		: IEmptySceneNode(parent, mgr, tunnel_border)
	{
		base::Ellipse ellipseFirst(core::vector2df(0,0), 140, 120, 0);
		m_dqTunnelModules.push_back(new TunnelModule(this, mgr, core::vector3df(0,0,-TUNNEL_MODULE_DIM_Z), ellipseFirst, ellipseFirst));
		for(int i=0 ; i<=9 ; i++)
		{
			const base::Ellipse* ellipseLast = m_dqTunnelModules.back()->GetEndingEllipse();
			TunnelModule* tunnel = new TunnelModule(this, mgr, core::vector3df(0,0,i*TUNNEL_MODULE_DIM_Z), *ellipseLast, RandomizeEllipse(*ellipseLast) );
			m_dqTunnelModules.push_back(tunnel);
		}
	}


	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	Tunnel::~Tunnel()
	{
		for(unsigned int i=0 ; i<m_dqTunnelModules.size() ; i++)
		{
			m_dqTunnelModules[i]->remove();
			delete m_dqTunnelModules[i];
		}
	}


	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	void Tunnel::OnAnimate(u32 timeMs)
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
			const base::Ellipse* ellipseLast = m_dqTunnelModules.back()->GetEndingEllipse();
			m_dqTunnelModules.push_back(new TunnelModule(this, getSceneManager(), core::vector3df(0,0,fZ+TUNNEL_MODULE_DIM_Z*10)-getAbsolutePosition(), *ellipseLast, RandomizeEllipse(*ellipseLast)));
		}
	}


	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	bool Tunnel::GetIsInTunnel(const core::vector3df& posAbs, scene::ISceneNode** outCheckedTunnel)const
	{
		core::vector3df posCheckRelTunnel = posAbs - getAbsolutePosition();

		int nPos = (posCheckRelTunnel.Z+300) / TUNNEL_MODULE_DIM_Z;
		if(nPos<0)return true;
		else if(nPos>10)
		{
			if(outCheckedTunnel!=0)
				*outCheckedTunnel = m_dqTunnelModules.back();
			return false;
		}

		TunnelModule* tunnelmod = m_dqTunnelModules[nPos];
		float posTunnelEntry = tunnelmod->getAbsolutePosition().Z;

		// @note (crom#1#): Clean this
		if(!(posTunnelEntry<posAbs.Z))
		{
			tunnelmod = m_dqTunnelModules[--nPos];
			posTunnelEntry = tunnelmod->getAbsolutePosition().Z;
		}
		else if(!(posTunnelEntry+300>posAbs.Z))
		{
			tunnelmod = m_dqTunnelModules[++nPos];
			posTunnelEntry = tunnelmod->getAbsolutePosition().Z;
		}

		if(outCheckedTunnel!=0)
			*outCheckedTunnel = tunnelmod;

		return tunnelmod->GetIsInTunnel(posCheckRelTunnel-tunnelmod->getPosition());
	}






	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	PRIVATE	PRIVATE	PRIVATE	PRIVATE	PRIVATE	PRIVATE	PRIVATE	PRIVATE	PRIVATE	PRIVATE	PRIVATE	PRIVATE	PRIVATE	PRIVATE	PRIV
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	base::Ellipse Tunnel::RandomizeEllipse(const base::Ellipse& base)
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

		return base::Ellipse(vCenter, fA, fB, fAngle);
	}



}
