#ifndef TUNNEL_MODULE_HPP_INCLUDED
#define TUNNEL_MODULE_HPP_INCLUDED

#include <cmath>
#include <vector>
#include <list>

#include <irrlicht.h>

#include "../base/Ellipse.hpp"

#include "../res.hpp"

#include "Object.hpp"

namespace game
{
	#define TUNNEL_MODULE_DIM_Z 300.0
	#define TUNNEL_MODULE_DIM_XY 300.0
	#define TUNNEL_MODULE_DIM_XY_HALF 150.0

	#define TUNNEL_TEXT_SCALE_X 200.0
	#define TUNNEL_TEXT_SCALE_Z 500.0

	class TunnelModule : public scene::ISceneNode
	{
	public:
		TunnelModule(scene::ISceneNode* parent, scene::ISceneManager* mgr, const core::vector3df& pos, const base::Ellipse& start, const base::Ellipse& end);


		virtual void OnRegisterSceneNode();

		virtual void render();

		virtual const core::aabbox3d<f32>& getBoundingBox() const{return m_MeshBuffer.BoundingBox;}
		virtual u32 getMaterialCount() const{return 2;}
		virtual video::SMaterial& getMaterial(u32 i){return m_MeshBuffer.Material;}

		bool GetIsInTunnel(const core::vector3df& vCheckPosRelNode);

		const base::Ellipse* GetStartingEllipse(){return &m_StartingEllipse;}
		const base::Ellipse* GetEndingEllipse(){return &m_EndingEllipse;}


	private:
		void AddFrontPolygon(const core::vector3df& a, const core::vector3df& b, const core::vector3df& c, const core::vector3df& d);

		void AddQuad(u16 A, u16 B, u16 C, u16 D);

		base::Ellipse m_StartingEllipse;
		base::Ellipse m_EndingEllipse;

		scene::SMesh m_Mesh;
		scene::CMeshBuffer<video::S3DVertexTangents> m_MeshBuffer;


		video::SColor clr;
	};
}

#endif // TUNNEL_MODULE_HPP_INCLUDED
