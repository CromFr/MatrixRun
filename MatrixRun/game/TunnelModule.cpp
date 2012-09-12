#include "TunnelModule.hpp"

using namespace std;
using namespace irr;

namespace game
{



	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	TunnelModule::TunnelModule(scene::ISceneNode* parent, scene::ISceneManager* mgr, const core::vector3df& pos, const base::Ellipse& start, const base::Ellipse& end)
		: ISceneNode(parent, mgr, tunnel_border, pos),
		clr(128,128,128,128)
	{
		m_StartingEllipse = start;
		m_EndingEllipse = end;


		//============================> Preparing mesh
		m_MeshBuffer.BoundingBox.reset(core::aabbox3d<f32>(-150,-150,0,300,300,300));


		m_MeshBuffer.Material.setTexture(0, res::material::Get("cave.jpg"));
		m_MeshBuffer.Material.setTexture(1, res::material::Get("rockwall_n.bmp"));
		m_MeshBuffer.Material.MaterialType = video::EMT_PARALLAX_MAP_SOLID;
		m_MeshBuffer.Material.MaterialTypeParam = 0.035;
		m_MeshBuffer.Material.Lighting = true;
		m_MeshBuffer.Material.FogEnable = true;
		//m_MeshBuffer.Material.Wireframe = true;

		m_Mesh.addMeshBuffer(&m_MeshBuffer);


//		//============================> Set the front polygons
//		list<core::vector2df>* listPoint = m_StartingEllipse.GetCalculatedPoints();
//		list<core::vector2df>::iterator it = listPoint->begin();
//
//
//		//middle line params : f(x) = fCoeff*(x+fXDiff)+fYDiff
//		//	Used to determine if the point must be linked to the top or bottom
//		float fCoeff = (m_StartingEllipse.GetCenter().Y-it->Y)/(m_StartingEllipse.GetCenter().X-it->X);
//		float fXDiff = -m_StartingEllipse.GetCenter().X;
//		float fYDiff = m_StartingEllipse.GetCenter().Y;
//
//
//		//Left border quads :
//		AddFrontPolygon(core::vector3df(-150,-150,0),
//						core::vector3df(-150,150,0),
//						core::vector3df(it->X,150,0),
//						core::vector3df(it->X,-150,0));
//
//
//		core::vector2df* vLastPointTop = &(*it);
//		core::vector2df* vLastPointBottom = &(*it);
//		it++;
//		for( ; it!=listPoint->end() ; it++)
//		{
//			//if fDelta>0, the point will be linked with upper side
//			float fDelta = it->Y - (fCoeff*(it->X+fXDiff)+fYDiff);
//
//			if(fDelta >= -0.05)// +/- 0.05 error due to float approx
//			{
//				AddFrontPolygon(core::vector3df(vLastPointTop->X,vLastPointTop->Y,0),
//								core::vector3df(vLastPointTop->X,150,0),
//								core::vector3df(it->X,150,0),
//								core::vector3df(it->X,it->Y,0));
//
//				vLastPointTop = &(*it);
//			}
//			if(fDelta <= 0.05)// +/- 0.05 error due to float approx
//			{
//				AddFrontPolygon(core::vector3df(it->X,it->Y,0),
//								core::vector3df(it->X,-150,0),
//								core::vector3df(vLastPointBottom->X,-150,0),
//								core::vector3df(vLastPointBottom->X,vLastPointBottom->Y,0));
//
//				vLastPointBottom = &(*it);
//			}
//
//		}
//		//Right border
//		AddFrontPolygon(core::vector3df(vLastPointBottom->X,-150,0),
//						core::vector3df(vLastPointBottom->X,150,0),
//						core::vector3df(150,150,0),
//						core::vector3df(150,-150,0));



		//============================> Set the tunnel polygons
		list<core::vector2df>* listStartPoint = m_StartingEllipse.GetCalculatedPoints();
		list<core::vector2df>::iterator itStart = listStartPoint->begin();
		list<core::vector2df>* listEndPoint = m_EndingEllipse.GetCalculatedPoints();
		list<core::vector2df>::iterator itEnd = listEndPoint->begin();


		//==> Find all upper/lower points

		//middle line params : f(x) = fStartCoeff*(x+fStartXDiff)+fStartYDiff
		//	Used to determine if the point must be linked to the top or bottom
		float fStartCoeff = (m_StartingEllipse.GetCenter().Y-itStart->Y)/(m_StartingEllipse.GetCenter().X-itStart->X);
		float fStartXDiff = -m_StartingEllipse.GetCenter().X;
		float fStartYDiff = m_StartingEllipse.GetCenter().Y;

		//The arrays that contain the points
		int nStartPoints = 2*m_StartingEllipse.GetCalcPrecision()-1;
		core::vector2df* aStartTopPoints[nStartPoints];
		//unsigned int aStartTopIndex[nStartPoints];
		core::vector2df* aStartBottomPoints[nStartPoints];
		//unsigned int aStartBottomIndex[nStartPoints];
		for(int iTop=0, iBot=0, nIndex=0 ; itStart!=listStartPoint->end() ; itStart++, nIndex++)
		{
			//fDelta represents if the point is at the top/bottom of the ellipse
			float fDelta = itStart->Y - (fStartCoeff*(itStart->X+fStartXDiff)+fStartYDiff);
			if(fDelta >= -0.05)// +/- 0.05 error due to float approx
			{
				aStartTopPoints[iTop] = &(*itStart);
				iTop++;
			}
			if(fDelta <= 0.05)// +/- 0.05 error due to float approx
			{
				aStartBottomPoints[iBot] = &(*itStart);
				iBot++;
			}
		}

		//idem for the bottom
		float fEndCoeff = (m_EndingEllipse.GetCenter().Y-itEnd->Y)/(m_EndingEllipse.GetCenter().X-itEnd->X);
		float fEndXDiff = -m_EndingEllipse.GetCenter().X;
		float fEndYDiff = m_EndingEllipse.GetCenter().Y;

		int nEndPoints = 2*m_EndingEllipse.GetCalcPrecision()-1;
		core::vector2df* aEndTopPoints[nEndPoints];
		//unsigned int aEndTopIndex[nStartPoints];
		core::vector2df* aEndBottomPoints[nEndPoints];
		//unsigned int aEndBottomIndex[nStartPoints];
		for(int iTop=0, iBot=0, nIndex=0 ; itEnd!=listEndPoint->end() ; itEnd++, nIndex++)
		{
			float fDelta = itEnd->Y - (fEndCoeff*(itEnd->X+fEndXDiff)+fEndYDiff);
			//cout<<"fDelta = "<<fDelta<<"\t Coord="<<itEnd->X<<"\t"<<itEnd->Y<<endl;
			if(fDelta >= -0.05)
			{
				aEndTopPoints[iTop] = &(*itEnd);
				//aEndTopIndex[iTop] = nIndex;
				iTop++;
			}
			if(fDelta <= 0.05)
			{
				aEndBottomPoints[iBot] = &(*itEnd);
				//aEndBottomIndex[iBot] = nIndex;
				iBot++;
			}
		}


		//==> Link all points, making quads


		//m_MeshBuffer.Vertices must be empty !

		//Adding first two points
		m_MeshBuffer.Vertices.push_back(video::S3DVertexTangents(	aStartBottomPoints[0]->X,
												aStartBottomPoints[0]->Y,
												0,
												aStartBottomPoints[0]->Y - aStartBottomPoints[1]->Y,
												aStartBottomPoints[1]->X - aStartBottomPoints[0]->X,
												0,
												clr,
												aStartBottomPoints[0]->X/TUNNEL_TEXT_SCALE_X,
												0,
												aStartBottomPoints[1]->X - aStartBottomPoints[0]->X,
												aStartBottomPoints[1]->Y - aStartBottomPoints[0]->Y,
												0,
												aStartBottomPoints[0]->Y - aStartBottomPoints[1]->Y,
												aStartBottomPoints[1]->X - aStartBottomPoints[0]->X,
												0));
		m_MeshBuffer.Vertices.push_back(video::S3DVertexTangents(	aEndBottomPoints[0]->X,
												aEndBottomPoints[0]->Y,
												TUNNEL_MODULE_DIM_Z,
												aEndBottomPoints[0]->Y - aEndBottomPoints[1]->Y,
												aEndBottomPoints[1]->X - aEndBottomPoints[0]->X,
												0,
												clr,
												aEndBottomPoints[0]->X/TUNNEL_TEXT_SCALE_X,
												TUNNEL_MODULE_DIM_Z/TUNNEL_TEXT_SCALE_Z,
												aEndBottomPoints[1]->X - aEndBottomPoints[0]->X,
												aEndBottomPoints[1]->Y - aEndBottomPoints[0]->Y,
												0,
												aEndBottomPoints[0]->Y - aEndBottomPoints[1]->Y,
												aEndBottomPoints[1]->X - aEndBottomPoints[0]->X,
												0));



		u16 nLastStartTopIndex = 0;
		u16 nLastStartBotIndex = 0;
		u16 nLastEndTopIndex = 1;
		u16 nLastEndBotIndex = 1;

		int i;
		for(i=1 ; i<nStartPoints-1 ; i++)//The first and the last is not treated
		{

			u16 nBaseIndex(m_MeshBuffer.Vertices.size());

			//Starting ellipse, top points
			m_MeshBuffer.Vertices.push_back(video::S3DVertexTangents(	aStartTopPoints[i]->X,
													aStartTopPoints[i]->Y,
													0,
													aStartTopPoints[i]->Y - aStartTopPoints[i-1]->Y,
													aStartTopPoints[i-1]->X - aStartTopPoints[i]->X,
													0,
													clr,
													aStartTopPoints[i]->X/TUNNEL_TEXT_SCALE_X,
													0,
													aStartTopPoints[i]->X - aStartTopPoints[i-1]->X,
													aStartTopPoints[i]->Y - aStartTopPoints[i-1]->Y,
													0,
													aStartTopPoints[i]->Y - aStartTopPoints[i-1]->Y,
													aStartTopPoints[i-1]->X - aStartTopPoints[i]->X,
													0));
			//Ending ellipse, top points
			m_MeshBuffer.Vertices.push_back(video::S3DVertexTangents(	aEndTopPoints[i]->X,
													aEndTopPoints[i]->Y,
													TUNNEL_MODULE_DIM_Z,
													aEndTopPoints[i]->Y - aEndTopPoints[i-1]->Y,
													aEndTopPoints[i-1]->X - aEndTopPoints[i]->X,
													0,
													clr,
													aEndTopPoints[i]->X/TUNNEL_TEXT_SCALE_X,
													TUNNEL_MODULE_DIM_Z/TUNNEL_TEXT_SCALE_Z,
													aEndTopPoints[i]->X - aEndTopPoints[i-1]->X,
													aEndTopPoints[i]->Y - aEndTopPoints[i-1]->Y,
													0,
													aEndTopPoints[i]->Y - aEndTopPoints[i-1]->Y,
													aEndTopPoints[i-1]->X - aEndTopPoints[i]->X,
													0));

			//Starting ellipse, bottom points
			m_MeshBuffer.Vertices.push_back(video::S3DVertexTangents(	aStartBottomPoints[i]->X,
													aStartBottomPoints[i]->Y,
													0,
													aStartBottomPoints[i-1]->Y - aStartBottomPoints[i]->Y,
													aStartBottomPoints[i]->X - aStartBottomPoints[i-1]->X,
													0,
													clr,
													aStartBottomPoints[i]->X/TUNNEL_TEXT_SCALE_X,
													0,
													aStartBottomPoints[i]->X - aStartBottomPoints[i-1]->X,
													aStartBottomPoints[i]->Y - aStartBottomPoints[i-1]->Y,
													0,
													aStartBottomPoints[i-1]->Y - aStartBottomPoints[i]->Y,
													aStartBottomPoints[i]->X - aStartBottomPoints[i-1]->X,
													0));

			//Ending ellipse, bottom points
			m_MeshBuffer.Vertices.push_back(video::S3DVertexTangents(	aEndBottomPoints[i]->X,
													aEndBottomPoints[i]->Y,
													TUNNEL_MODULE_DIM_Z,
													aEndBottomPoints[i-1]->Y - aEndBottomPoints[i]->Y,
													aEndBottomPoints[i]->X - aEndBottomPoints[i-1]->X,
													0,
													clr,
													aEndBottomPoints[i]->X/TUNNEL_TEXT_SCALE_X,
													TUNNEL_MODULE_DIM_Z/TUNNEL_TEXT_SCALE_Z,
													aEndBottomPoints[i]->X - aEndBottomPoints[i-1]->X,
													aEndBottomPoints[i]->Y - aEndBottomPoints[i-1]->Y,
													0,
													aEndBottomPoints[i-1]->Y - aEndBottomPoints[i]->Y,
													aEndBottomPoints[i]->X - aEndBottomPoints[i-1]->X,
													0));

			//Top
			AddQuad(nLastStartTopIndex, nBaseIndex, nBaseIndex+1, nLastEndTopIndex);

			//Bottom
			AddQuad(nBaseIndex+2, nLastStartBotIndex, nLastEndBotIndex, nBaseIndex+3);



			nLastStartTopIndex = nBaseIndex;
			nLastEndTopIndex = nBaseIndex+1;
			nLastStartBotIndex = nBaseIndex+2;
			nLastEndBotIndex = nBaseIndex+3;
		}

		u16 nBaseIndex(m_MeshBuffer.Vertices.size());
		//Starting ellipse point
		m_MeshBuffer.Vertices.push_back(video::S3DVertexTangents(	aStartBottomPoints[i]->X,
												aStartBottomPoints[i]->Y,
												0,
												aStartBottomPoints[i-1]->Y - aStartBottomPoints[i]->Y,
												aStartBottomPoints[i]->X - aStartBottomPoints[i-1]->X,
												0,
												clr,
												aStartBottomPoints[i]->X/TUNNEL_TEXT_SCALE_X,
												0,
												aStartBottomPoints[i]->X - aStartBottomPoints[i-1]->X,
												aStartBottomPoints[i]->Y - aStartBottomPoints[i-1]->Y,
												0,
												aStartBottomPoints[i-1]->Y - aStartBottomPoints[i]->Y,
												aStartBottomPoints[i]->X - aStartBottomPoints[i-1]->X,
												0));

		//Ending ellipse point
		m_MeshBuffer.Vertices.push_back(video::S3DVertexTangents(	aEndBottomPoints[i]->X,
												aEndBottomPoints[i]->Y,
												TUNNEL_MODULE_DIM_Z,
												aEndBottomPoints[i-1]->Y - aEndBottomPoints[i]->Y,
												aEndBottomPoints[i]->X - aEndBottomPoints[i-1]->X,
												0,
												clr,
												aEndBottomPoints[i]->X/TUNNEL_TEXT_SCALE_X,
												TUNNEL_MODULE_DIM_Z/TUNNEL_TEXT_SCALE_Z,
												aEndBottomPoints[i]->X - aEndBottomPoints[i-1]->X,
												aEndBottomPoints[i]->Y - aEndBottomPoints[i-1]->Y,
												0,
												aEndBottomPoints[i-1]->Y - aEndBottomPoints[i]->Y,
												aEndBottomPoints[i]->X - aEndBottomPoints[i-1]->X,
												0));

		AddQuad(nLastStartTopIndex, nBaseIndex, nBaseIndex+1, nLastEndTopIndex);
		AddQuad(nBaseIndex, nLastStartBotIndex, nLastEndBotIndex, nBaseIndex+1);

	}


	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	void TunnelModule::OnRegisterSceneNode()
	{
		if(IsVisible)
			SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}

	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	void TunnelModule::render()
	{
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setMaterial(m_MeshBuffer.Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

		driver->drawVertexPrimitiveList(&m_MeshBuffer.Vertices[0], m_MeshBuffer.Vertices.size(), &m_MeshBuffer.Indices[0], m_MeshBuffer.Indices.size()/4, video::EVT_TANGENTS, scene::EPT_QUADS, video::EIT_16BIT);
	}

	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	bool TunnelModule::GetIsInTunnel(const core::vector3df& vCheckPosRelNode)
	{
		//fCoeff = 0:start, 1:end
		float fCoeff = vCheckPosRelNode.Z/TUNNEL_MODULE_DIM_Z;
		float fAntiCoeff = 1-fCoeff;

		base::Ellipse ellipseMidle(	m_StartingEllipse.GetCenter()*fCoeff + m_EndingEllipse.GetCenter()*fAntiCoeff,
									m_StartingEllipse.GetA()*fCoeff + m_EndingEllipse.GetA()*fAntiCoeff,
									m_StartingEllipse.GetB()*fCoeff + m_EndingEllipse.GetB()*fAntiCoeff,
									m_StartingEllipse.GetAngle()*fCoeff + m_EndingEllipse.GetAngle()*fAntiCoeff);

		return ellipseMidle.GetIsInto(vCheckPosRelNode.X, vCheckPosRelNode.Y);
	}






	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	PRIVATE ==========================
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	void TunnelModule::AddFrontPolygon(const core::vector3df& a, const core::vector3df& b, const core::vector3df& c, const core::vector3df& d)
	{
		int nBaseIndex = m_MeshBuffer.Vertices.size();
		core::vector3df normal(0,0,0);

		m_MeshBuffer.Vertices.push_back(video::S3DVertexTangents(	a,	normal,		clr,	core::vector2df(a.X/100.0,a.Y/100.0)));
		m_MeshBuffer.Vertices.push_back(video::S3DVertexTangents(	b,	normal,		clr,	core::vector2df(b.X/100.0,b.Y/100.0)));
		m_MeshBuffer.Vertices.push_back(video::S3DVertexTangents(	c,	normal,		clr,	core::vector2df(c.X/100.0,c.Y/100.0)));
		m_MeshBuffer.Vertices.push_back(video::S3DVertexTangents(	d,	normal,		clr,	core::vector2df(d.X/100.0,d.Y/100.0)));

		m_MeshBuffer.Indices.push_back(nBaseIndex);
		m_MeshBuffer.Indices.push_back(nBaseIndex+1);
		m_MeshBuffer.Indices.push_back(nBaseIndex+2);
		m_MeshBuffer.Indices.push_back(nBaseIndex+3);
	}

	/*=================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	=================================================================================================================*/
	void TunnelModule::AddQuad(u16 A, u16 B, u16 C, u16 D)
	{
		m_MeshBuffer.Indices.push_back(A);
		m_MeshBuffer.Indices.push_back(B);
		m_MeshBuffer.Indices.push_back(C);
		m_MeshBuffer.Indices.push_back(D);
	}

}
