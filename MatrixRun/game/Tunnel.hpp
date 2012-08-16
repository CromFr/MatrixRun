#ifndef TUNNEL_HPP_INCLUDED
#define TUNNEL_HPP_INCLUDED

#include <cmath>
#include <vector>
#include <list>
using namespace std;

#include <irrlicht.h>
using namespace irr;

#include "../base/Elipse.hpp"

namespace game
{

	class Tunnel : public scene::ISceneNode
	{
	public:
		Tunnel(scene::ISceneNode* parent, scene::ISceneManager* mgr, const core::vector3df& pos, const base::Elipse& start, const base::Elipse& end)
			: scene::ISceneNode(parent, mgr, tunnel_border, pos),
			m_AABBox(-150,-150,0,300,300,300),
			clr(128,128,128,128)
		{
			m_StartingElipse = start;
			m_EndingElipse = end;


			m_Material.setTexture(0, res::material::Get("cave.jpg"));
			m_Material.Lighting = true;
			m_Material.FogEnable = true;
			//m_Material.Wireframe = true;


//			//============================> Set the front polygons
//			list<core::vector2df>* listPoint = m_StartingElipse.GetCalculatedPoints();
//			list<core::vector2df>::iterator it = listPoint->begin();
//
//
//			//middle line params : f(x) = fCoeff*(x+fXDiff)+fYDiff
//			//	Used to determine if the point must be linked to the top or bottom
//			float fCoeff = (m_StartingElipse.GetCenter().Y-it->Y)/(m_StartingElipse.GetCenter().X-it->X);
//			float fXDiff = -m_StartingElipse.GetCenter().X;
//			float fYDiff = m_StartingElipse.GetCenter().Y;
//
//
//			//Left border quads :
//			AddFrontPolygon(core::vector3df(-150,-150,0),
//							core::vector3df(-150,150,0),
//							core::vector3df(it->X,150,0),
//							core::vector3df(it->X,-150,0));
//
//
//			core::vector2df* vLastPointTop = &(*it);
//			core::vector2df* vLastPointBottom = &(*it);
//			it++;
//			for( ; it!=listPoint->end() ; it++)
//			{
//				//if fDelta>0, the point will be linked with upper side
//				float fDelta = it->Y - (fCoeff*(it->X+fXDiff)+fYDiff);
//
//				if(fDelta >= -0.05)// +/- 0.05 error due to float approx
//				{
//					AddFrontPolygon(core::vector3df(vLastPointTop->X,vLastPointTop->Y,0),
//									core::vector3df(vLastPointTop->X,150,0),
//									core::vector3df(it->X,150,0),
//									core::vector3df(it->X,it->Y,0));
//
//					vLastPointTop = &(*it);
//				}
//				if(fDelta <= 0.05)// +/- 0.05 error due to float approx
//				{
//					AddFrontPolygon(core::vector3df(it->X,it->Y,0),
//									core::vector3df(it->X,-150,0),
//									core::vector3df(vLastPointBottom->X,-150,0),
//									core::vector3df(vLastPointBottom->X,vLastPointBottom->Y,0));
//
//					vLastPointBottom = &(*it);
//				}
//
//			}
//			//Right border
//			AddFrontPolygon(core::vector3df(vLastPointBottom->X,-150,0),
//							core::vector3df(vLastPointBottom->X,150,0),
//							core::vector3df(150,150,0),
//							core::vector3df(150,-150,0));



			//============================> Set the tunnel polygons
			int iTop=0, iBot=0;
			list<core::vector2df>* listStartPoint = m_StartingElipse.GetCalculatedPoints();
			list<core::vector2df>::iterator itStart = listStartPoint->begin();
			list<core::vector2df>* listEndPoint = m_EndingElipse.GetCalculatedPoints();
			list<core::vector2df>::iterator itEnd = listEndPoint->begin();


			//==> Find all upper/lower points

			//middle line params : f(x) = fStartCoeff*(x+fStartXDiff)+fStartYDiff
			//	Used to determine if the point must be linked to the top or bottom
			float fStartCoeff = (m_StartingElipse.GetCenter().Y-itStart->Y)/(m_StartingElipse.GetCenter().X-itStart->X);
			float fStartXDiff = -m_StartingElipse.GetCenter().X;
			float fStartYDiff = m_StartingElipse.GetCenter().Y;

			//The arrays that contain the points
			int nStartPoints = 2*m_StartingElipse.GetCalcPrecision()-1;
			core::vector2df* aStartTopPoints[nStartPoints];
			core::vector2df* aStartBottomPoints[nStartPoints];
			for(iTop=0, iBot=0 ; itStart!=listStartPoint->end() ; itStart++)
			{
				//fDelta represents if the point is at the top/bottom of the elipse
				float fDelta = itStart->Y - (fStartCoeff*(itStart->X+fStartXDiff)+fStartYDiff);
				if(fDelta >= -0.05)// +/- 0.05 error due to float approx
				{
					aStartTopPoints[iTop++] = &(*itStart);
				}
				if(fDelta <= 0.05)// +/- 0.05 error due to float approx
				{
					aStartBottomPoints[iBot++] = &(*itStart);
				}
			}

			//idem for the bottom
			float fEndCoeff = (m_EndingElipse.GetCenter().Y-itEnd->Y)/(m_EndingElipse.GetCenter().X-itEnd->X);
			float fEndXDiff = -m_EndingElipse.GetCenter().X;
			float fEndYDiff = m_EndingElipse.GetCenter().Y;

			int nEndPoints = 2*m_EndingElipse.GetCalcPrecision()-1;
			core::vector2df* aEndTopPoints[nEndPoints];
			core::vector2df* aEndBottomPoints[nEndPoints];
			for(iTop=0, iBot=0 ; itEnd!=listEndPoint->end() ; itEnd++)
			{
				float fDelta = itEnd->Y - (fEndCoeff*(itEnd->X+fEndXDiff)+fEndYDiff);
				//cout<<"fDelta = "<<fDelta<<"\t Coord="<<itEnd->X<<"\t"<<itEnd->Y<<endl;
				if(fDelta >= -0.05)
				{
					aEndTopPoints[iTop++] = &(*itEnd);
				}
				if(fDelta <= 0.05)
				{
					aEndBottomPoints[iBot++] = &(*itEnd);
				}
			}


			//==> Link all points, making quads
			for(int i=0 ; i<nStartPoints-1 ; i++)
			{
				AddTunnelQuad(
								aStartTopPoints[i]->X,		aStartTopPoints[i]->Y,		0,
								aStartTopPoints[i+1]->X,	aStartTopPoints[i+1]->Y,	0,
								aEndTopPoints[i+1]->X,		aEndTopPoints[i+1]->Y,		300,
								aEndTopPoints[i]->X,		aEndTopPoints[i]->Y,		300,
								1);

				AddTunnelQuad(
								aEndBottomPoints[i]->X,		aEndBottomPoints[i]->Y,		300,
								aEndBottomPoints[i+1]->X,	aEndBottomPoints[i+1]->Y,	300,
								aStartBottomPoints[i+1]->X,	aStartBottomPoints[i+1]->Y,	0,
								aStartBottomPoints[i]->X,	aStartBottomPoints[i]->Y,	0,
								-1);
			}
		}


		//============================================
		virtual void OnRegisterSceneNode()
		{
			if(IsVisible)
				SceneManager->registerNodeForRendering(this);
			ISceneNode::OnRegisterSceneNode();
		}

		//============================================
		virtual void render()
		{
			video::IVideoDriver* driver = SceneManager->getVideoDriver();

			driver->setMaterial(m_Material);
			driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

			driver->drawVertexPrimitiveList(&m_vVertices[0], m_vVertices.size(), &m_vPoligons[0], m_vPoligons.size()/4, video::EVT_STANDARD, scene::EPT_QUADS, video::EIT_16BIT);
		}

		virtual const core::aabbox3d<f32>& getBoundingBox() const{return m_AABBox;}
		virtual u32 getMaterialCount() const{return 1;}
		virtual video::SMaterial& getMaterial(u32 i){return m_Material;}

		//============================================
		bool GetIsInTunnel(const core::vector3df& vCheckPosAbs)
		{
			core::vector3df vCheckPosRelNode(vCheckPosAbs-getAbsolutePosition());
			if(!m_AABBox.isPointInside(vCheckPosRelNode))
				return false;

			//fCoeff = 0:start, 1:end
			float fCoeff = vCheckPosRelNode.Z/300.0;
			float fAntiCoeff = 1-fCoeff;
			base::Elipse elipseMidle(	m_StartingElipse.GetCenter()*fCoeff + m_EndingElipse.GetCenter()*fAntiCoeff,
										m_StartingElipse.GetA()*fCoeff + m_EndingElipse.GetA()*fAntiCoeff,
										m_StartingElipse.GetB()*fCoeff + m_EndingElipse.GetB()*fAntiCoeff,
										m_StartingElipse.GetAngle()*fCoeff + m_EndingElipse.GetAngle()*fAntiCoeff);

			return elipseMidle.GetIsInto(vCheckPosRelNode.X, vCheckPosRelNode.Y);
		}

		const base::Elipse* GetStartingElipse(){return &m_StartingElipse;}
		const base::Elipse* GetEndingElipse(){return &m_EndingElipse;}


//============================================
	private:
		//============================================
		void AddFrontPolygon(const core::vector3df& a, const core::vector3df& b, const core::vector3df& c, const core::vector3df& d)
		{
			int nBaseIndex = m_vVertices.size();
			core::vector3df normal(0,0,0);

			m_vVertices.push_back(video::S3DVertex(	a,	normal,		clr,	core::vector2df(a.X/100.0,a.Y/100.0)));
			m_vVertices.push_back(video::S3DVertex(	b,	normal,		clr,	core::vector2df(b.X/100.0,b.Y/100.0)));
			m_vVertices.push_back(video::S3DVertex(	c,	normal,		clr,	core::vector2df(c.X/100.0,c.Y/100.0)));
			m_vVertices.push_back(video::S3DVertex(	d,	normal,		clr,	core::vector2df(d.X/100.0,d.Y/100.0)));

			m_vPoligons.push_back(nBaseIndex);
			m_vPoligons.push_back(nBaseIndex+1);
			m_vPoligons.push_back(nBaseIndex+2);
			m_vPoligons.push_back(nBaseIndex+3);
		}

		//============================================
		//the two first vectors are used for the normal
		void AddTunnelQuad( const float& fX1, const float& fY1, const float& fZ1,
							const float& fX2, const float& fY2, const float& fZ2,
							const float& fX3, const float& fY3, const float& fZ3,
							const float& fX4, const float& fY4, const float& fZ4,
							signed short nNormal)
		{

			int nBaseIndex = m_vVertices.size();
			m_vVertices.push_back(video::S3DVertex(fX1,fY1,fZ1,  0,0,0,  clr, fX1/200.0, fZ1/500.0));
			m_vVertices.push_back(video::S3DVertex(fX2,fY2,fZ2,  0,0,0,  clr, fX2/200.0, fZ2/500.0));
			m_vVertices.push_back(video::S3DVertex(fX3,fY3,fZ3,  0,0,0,  clr, fX3/200.0, fZ3/500.0));
			m_vVertices.push_back(video::S3DVertex(fX4,fY4,fZ4,  0,0,0,  clr, fX4/200.0, fZ4/500.0));

			for(int i=0 ; i<4 ; i++)
				m_vPoligons.push_back(nBaseIndex+i);



		}

		base::Elipse m_StartingElipse;
		base::Elipse m_EndingElipse;


		core::aabbox3d<f32> m_AABBox;
		vector<video::S3DVertex> m_vVertices;
		vector<u16> m_vPoligons;
		video::SMaterial m_Material;


		video::SColor clr;
	};
}

#endif // TUNNEL_HPP_INCLUDED
