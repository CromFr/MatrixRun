#ifndef VFX_MANAGER_HPP_INCLUDED
#define VFX_MANAGER_HPP_INCLUDED

#include <vector>

#include <irrlicht.h>

#include <SPARK/SPK.h>
#include <SPARK/SPK_IRR.h>

#include "../res.hpp"

namespace game
{


	/**
	@brief Static class to handle loading, adding/stopping visual effects
	**/
	class VfxManager
	{
	public:
		enum vfx
		{
			bullet_shot=0,
			bullet_trail=1,
			bullet_impact=2,
			blood_sentinel=3
		};

		static void Init(irr::IrrlichtDevice* oDev);
		static SPK::IRR::IRRSystem* AddVfxSceneNode(irr::scene::ISceneNode* parent, enum vfx effect, const irr::core::vector3df& position=irr::core::vector3df(0,0,0));
		static void StopEmitting(SPK::IRR::IRRSystem* sys);
	private:
		static std::vector<SPK::Registerable*> m_vfxList;
	};





}


#endif // VFX_MANAGER_HPP_INCLUDED
