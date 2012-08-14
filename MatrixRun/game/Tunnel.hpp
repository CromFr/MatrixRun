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
			: scene::ISceneNode(parent, mgr, -1, pos),
			//m_StartingElipse(core::vector2df(0,0), 140, 80, 0),
			//m_EndingElipse(core::vector2df(-50,-90), 100, 70, 30),
			m_AABBox(-150,-150,0,300,300,300),
			clr(128,128,128,128)
		{
			m_StartingElipse = start;
			m_EndingElipse = end;


			m_Material.setTexture(0, SceneManager->getVideoDriver()->getTexture("../MatrixRun/data/model/wood.jpg"));
			m_Material.Lighting = true;
			m_Material.FogEnable = true;
			//m_Material.Wireframe = true;


			//============================> Set the front polygons
			list<core::vector2df>* listPoint = m_StartingElipse.GetCalculatedPoints();
			list<core::vector2df>::iterator it = listPoint->begin();


			//middle line params : f(x) = fCoeff*(x+fXDiff)+fYDiff
			//	Used to determine if the point must be linked to the top or bottom
			float fCoeff = (m_StartingElipse.GetCenter().Y-it->Y)/(m_StartingElipse.GetCenter().X-it->X);
			float fXDiff = -m_StartingElipse.GetCenter().X;
			float fYDiff = m_StartingElipse.GetCenter().Y;


			//Left border quads :
			AddFrontPolygon(core::vector3df(-150,-150,0),
							core::vector3df(-150,150,0),
							core::vector3df(it->X,150,0),
							core::vector3df(it->X,-150,0));


			core::vector2df* vLastPointTop = &(*it);
			core::vector2df* vLastPointBottom = &(*it);
			it++;
			for( ; it!=listPoint->end() ; it++)
			{
				//if fDelta>0, the point will be linked with upper side
				float fDelta = it->Y - (fCoeff*(it->X+fXDiff)+fYDiff);

				if(fDelta >= -0.05)// +/- 0.05 error due to float approx
				{
					AddFrontPolygon(core::vector3df(vLastPointTop->X,vLastPointTop->Y,0),
									core::vector3df(vLastPointTop->X,150,0),
									core::vector3df(it->X,150,0),
									core::vector3df(it->X,it->Y,0));

					vLastPointTop = &(*it);
				}
				if(fDelta <= 0.05)// +/- 0.05 error due to float approx
				{
					AddFrontPolygon(core::vector3df(it->X,it->Y,0),
									core::vector3df(it->X,-150,0),
									core::vector3df(vLastPointBottom->X,-150,0),
									core::vector3df(vLastPointBottom->X,vLastPointBottom->Y,0));

					vLastPointBottom = &(*it);
				}

			}
			//Right border
			AddFrontPolygon(core::vector3df(vLastPointBottom->X,-150,0),
							core::vector3df(vLastPointBottom->X,150,0),
							core::vector3df(150,150,0),
							core::vector3df(150,-150,0));



			//============================> Set the tunnel polygons
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
			core::vector2df* aStartTopPoints[m_StartingElipse.GetCalcPrecision()*2];
			core::vector2df* aStartBottomPoints[m_StartingElipse.GetCalcPrecision()*2];
			for(int iTop=0, iBot=0 ; itStart!=listStartPoint->end() ; itStart++)
			{
				//fDelta represents if the point is at the top/bottom of the elipse
				float fDelta = itStart->Y - (fStartCoeff*(itStart->X+fStartXDiff)+fStartYDiff);
				if(fDelta >= -0.5)// +/- 0.05 error due to float approx
				{
					aStartTopPoints[iTop++] = &(*itStart);
				}
				if(fDelta <= 0.5)// +/- 0.05 error due to float approx
				{
					aStartBottomPoints[iBot++] = &(*itStart);
				}
			}

			//idem for the bottom
			float fEndCoeff = (m_EndingElipse.GetCenter().Y-itEnd->Y)/(m_EndingElipse.GetCenter().X-itEnd->X);
			float fEndXDiff = -m_EndingElipse.GetCenter().X;
			float fEndYDiff = m_EndingElipse.GetCenter().Y;

			core::vector2df* aEndTopPoints[m_EndingElipse.GetCalcPrecision()*2];
			core::vector2df* aEndBottomPoints[m_EndingElipse.GetCalcPrecision()*2];
			for(int iTop=0, iBot=0 ; itEnd!=listEndPoint->end() ; itEnd++)
			{
				float fDelta = itEnd->Y - (fEndCoeff*(itEnd->X+fEndXDiff)+fEndYDiff);
				if(fDelta >= -0.5)
				{
					aEndTopPoints[iTop++] = &(*itEnd);
				}
				if(fDelta <= 0.5)
				{
					aEndBottomPoints[iBot++] = &(*itEnd);
				}
			}


			//==> Link all points, making quads
			for(int i=0 ; i<m_StartingElipse.GetCalcPrecision()*2+2 ; i++)// @fixme (crom#1#): why +2? =D
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



		virtual void OnRegisterSceneNode()
		{
			if(IsVisible)
				SceneManager->registerNodeForRendering(this);
			ISceneNode::OnRegisterSceneNode();
		}

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

	private:
		void AddFrontPolygon(const core::vector3df& a, const core::vector3df& b, const core::vector3df& c, const core::vector3df& d)
		{
			int nBaseIndex = m_vVertices.size();

			//core::vector3df normal(0,0,-1);
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

		//the two first vectors are used for the normal
		void AddTunnelQuad( const float& fX1, const float& fY1, const float& fZ1,
							const float& fX2, const float& fY2, const float& fZ2,
							const float& fX3, const float& fY3, const float& fZ3,
							const float& fX4, const float& fY4, const float& fZ4,
							signed short nNormal)
		{
	//		float fNormX=nNormal, fNormY=nNormal;
	//
	//		fNormX *= (fY1-fY2);
	//		fNormY *= (fX1-fX2);

			int nBaseIndex = m_vVertices.size();
	//
	//		m_vVertices.push_back(video::S3DVertex(fX1,fY1,fZ1,  fNormX,fNormY,0,  clr, fX1/100, fZ1/100));
	//		m_vVertices.push_back(video::S3DVertex(fX2,fY2,fZ2,  fNormX,fNormY,0,  clr, fX2/100, fZ2/100));
	//		m_vVertices.push_back(video::S3DVertex(fX3,fY3,fZ3,  fNormX,fNormY,0,  clr, fX3/100, fZ3/100));
	//		m_vVertices.push_back(video::S3DVertex(fX4,fY4,fZ4,  fNormX,fNormY,0,  clr, fX4/100, fZ4/100));
			m_vVertices.push_back(video::S3DVertex(fX1,fY1,fZ1,  0,0,0,  clr, fX1/100, fZ1/100));
			m_vVertices.push_back(video::S3DVertex(fX2,fY2,fZ2,  0,0,0,  clr, fX2/100, fZ2/100));
			m_vVertices.push_back(video::S3DVertex(fX3,fY3,fZ3,  0,0,0,  clr, fX3/100, fZ3/100));
			m_vVertices.push_back(video::S3DVertex(fX4,fY4,fZ4,  0,0,0,  clr, fX4/100, fZ4/100));

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
