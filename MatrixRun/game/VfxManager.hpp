#ifndef VFX_MANAGER_HPP_INCLUDED
#define VFX_MANAGER_HPP_INCLUDED

#include <iostream>
#include <vector>

#include <irrlicht.h>

#include <SPK.h>
#include <SPK_IRR.h>

#include "../res.hpp"

namespace game
{

	enum vfx
	{
		vfx_bullet_shot=0,
		vfx_bullet_trail=1,
		vfx_bullet_impact=2,
		vfx_blood_sentinel=3
	};

	class VfxManager
	{
	public:
		static void Init(irr::IrrlichtDevice* oDev);
		static SPK::IRR::IRRSystem* AddVfxSceneNode(irr::scene::ISceneNode* parent, enum vfx effect, const irr::core::vector3df& position=irr::core::vector3df(0,0,0));
		static void StopEmitting(SPK::IRR::IRRSystem* sys);
	private:
		static std::vector<SPK::Registerable*> m_vfxList;
	};





}


#endif // VFX_MANAGER_HPP_INCLUDED
