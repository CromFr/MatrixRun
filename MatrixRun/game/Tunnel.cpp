#include "Tunnel.hpp"

#include "TunnelModule.hpp"

using namespace irr;
using namespace std;



//@todo : remove this, testing only (generating randomly sentinels)
#include "Sentinel.hpp"

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
		for(int i=0 ; i<10 ; i++)
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
		if(fZ<=-800)
		{
			//modulo position
			RelativeTranslation.Z+=TUNNEL_MODULE_DIM_Z;//+300z for the container
			for(core::list<ISceneNode*>::Iterator it = Children.begin() ; it!=Children.end() ; it++)//-300 for the children
			{
				core::vector3df newpos = (*it)->getPosition();
				newpos.Z-=TUNNEL_MODULE_DIM_Z;
				(*it)->setPosition(newpos);
			}
			//removing first module
			m_dqTunnelModules.front()->removeAll();
			m_dqTunnelModules.front()->remove();
			m_dqTunnelModules.pop_front();

			//Adding a new module
			const base::Ellipse* ellipseLast = m_dqTunnelModules.back()->GetEndingEllipse();
			m_dqTunnelModules.push_back(new TunnelModule(this, getSceneManager(), core::vector3df(0,0,fZ+TUNNEL_MODULE_DIM_Z*10)-getAbsolutePosition(), *ellipseLast, RandomizeEllipse(*ellipseLast)));


			//@todo : Remove this, testing only
			if(rand()%5 == 4)
			{
				irr::core::vector2df center(m_dqTunnelModules.back()->GetStartingEllipse()->GetCenter());

				new Sentinel(m_dqTunnelModules.back(), getSceneManager(), core::vector3df(center.X,center.Y,100));
			}
		}
	}


	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	TunnelModule* Tunnel::GetTunnelModuleAt(const double fAbsZ)const
	{
		double fRelZ = fAbsZ - getAbsolutePosition().Z;
		int nIndex = fRelZ/TUNNEL_MODULE_DIM_Z+1;

		if(0<=nIndex && nIndex<=10)
			return m_dqTunnelModules[nIndex];
		else
			return nullptr;
	}


	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	bool Tunnel::GetIsInTunnel(const core::vector3df& posAbs, scene::ISceneNode** outCheckedTunnel, core::vector3df* outCollPosAbs)const
	{
		//@todo: This needs some optimizations
		TunnelModule* tunnelmod = GetTunnelModuleAt(posAbs.Z);

		//tunnelmod->RenderWireframes(true);

		if(outCheckedTunnel!=nullptr)
			*outCheckedTunnel = tunnelmod;

		if(tunnelmod!=nullptr)
			return tunnelmod->GetIsInTunnel(posAbs-tunnelmod->getPosition(), outCollPosAbs);
		else
		{
			//Handling tunnel bounds collisions
			double fRelZ = posAbs.Z - getAbsolutePosition().Z;
			int nIndex = fRelZ/TUNNEL_MODULE_DIM_Z+1;
			if(nIndex<0)
				return true;
			else if(nIndex>10)
				return false;
			else
				cerr<<"Error Tunnel::GetIsInTunnel : THIS SHOULD NOT HAPPEN"<<endl;
		}
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
