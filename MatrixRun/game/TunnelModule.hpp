#ifndef TUNNEL_MODULE_HPP_INCLUDED
#define TUNNEL_MODULE_HPP_INCLUDED

#include <irrlicht.h>

#include "../base/Ellipse.hpp"

#include "Object.hpp"

namespace game
{
	#define TUNNEL_MODULE_DIM_Z 300.0
	#define TUNNEL_MODULE_DIM_XY 300.0
	#define TUNNEL_MODULE_DIM_XY_HALF 150.0

	#define TUNNEL_TEXT_SCALE_X 200.0
	#define TUNNEL_TEXT_SCALE_Z 500.0

	/**
	@brief Small section of the tunnel, starting and ending with two given ellipsis
	**/
	class TunnelModule : public irr::scene::ISceneNode
	{
	public:
		TunnelModule(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, const irr::core::vector3df& pos, const base::Ellipse& start, const base::Ellipse& end);


		virtual void OnRegisterSceneNode() override;

		virtual void render() override;

		virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox()const override {return m_MeshBuffer.BoundingBox;}
		virtual irr::u32 getMaterialCount()const override {return 2;}
		virtual irr::video::SMaterial& getMaterial(irr::u32 i) override {return m_MeshBuffer.Material;}

		bool GetIsInTunnel(const irr::core::vector3df& vCheckPosRelNode, irr::core::vector3df* outCollisionPoint=0)const;

		const base::Ellipse* GetStartingEllipse()const {return &m_StartingEllipse;}
		const base::Ellipse* GetEndingEllipse()const {return &m_EndingEllipse;}


	private:
		void AddFrontPolygon(const irr::core::vector3df& a, const irr::core::vector3df& b, const irr::core::vector3df& c, const irr::core::vector3df& d);

		void AddQuad(irr::u16 A, irr::u16 B, irr::u16 C, irr::u16 D);

		base::Ellipse m_StartingEllipse;
		base::Ellipse m_EndingEllipse;

		irr::scene::SMesh m_Mesh;
		irr::scene::CMeshBuffer<irr::video::S3DVertexTangents> m_MeshBuffer;


		irr::video::SColor clr;
	};
}

#endif // TUNNEL_MODULE_HPP_INCLUDED
