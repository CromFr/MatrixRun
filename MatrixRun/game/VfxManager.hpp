#ifndef VFX_MANAGER_HPP_INCLUDED
#define VFX_MANAGER_HPP_INCLUDED

#include <iostream>
#include <vector>
using namespace std;

#include <irrlicht.h>
using namespace irr;

#include <SPK.h>
#include <SPK_IRR.h>
using namespace SPK;

#include "../res.hpp"

namespace game
{

	enum vfx
	{
		vfx_bullet_shot=0,
		vfx_bullet_trail=1,
		vfx_bullet_impact=2
	};

	class VfxManager
	{
	public:
		static void Init(IrrlichtDevice* oDev);
		static IRR::IRRSystem* AddVfxSceneNode(scene::ISceneNode* parent, enum vfx effect, const core::vector3df& position=core::vector3df(0,0,0));
		static void StopEmitting(IRR::IRRSystem* sys);
	private:
		static vector<Registerable*> m_vfxList;
	};





}


#endif // VFX_MANAGER_HPP_INCLUDED
