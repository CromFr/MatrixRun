#ifndef SENTINEL_HPP_INCLUDED
#define SENTINEL_HPP_INCLUDED

#include "Destroyable.hpp"
#include "../irr/IEmptySceneNode.h"

#include "VfxManager.hpp"

namespace game
{
	class Sentinel : public Destroyable, public scene::IEmptySceneNode
	{
	public:
		Sentinel(scene::ISceneNode* parent, scene::ISceneManager* mgr,
				const core::vector3df& position)
			: Destroyable(100),
			  IEmptySceneNode(parent, mgr, sentinel, position)
			//m_collisionnable(this, this->getSceneManager(), 0, bind(&Sentinel::OnCollision, this, _1, _2, _3))
		{

			//3d model
			m_model = mgr->addMeshSceneNode(res::model::Get("sentinel.3ds"), this);
			m_model->setMaterialFlag(video::EMF_LIGHTING, true);
			m_model->setMaterialFlag(video::EMF_FOG_ENABLE, true);
			m_model->setTriangleSelector(mgr->createTriangleSelector(res::model::Get("sentinel.3ds"), m_model));

			//Light
			m_light = mgr->addLightSceneNode(this, core::vector3df(0,0,2), video::SColor(255,255,0,0), 5);

			//Collisions
			//m_collisionnable.SetCollisionFlags(tunnel_border|tunnel_block|sentinel|ship);
			//m_collisionnable.AddCollisionPoint(core::vector3df(0,0,0));

		}



	private:
		virtual void OnDamaged(int nAmount)
		{
			cout<<"OnDamaged"<<endl;
			VfxManager::AddVfxSceneNode(this, vfx_blood_sentinel);
		}
		virtual void OnDeath()
		{

		}

		void OnCollision(const scene::ISceneNode* node, const core::triangle3df& triangle, const core::vector3df& position)
		{

		}



		enum ObjectType GetType(){return sentinel;}

		scene::IMeshSceneNode* m_model;
		scene::ILightSceneNode* m_light;
		//Collisionnable m_collisionnable;
	};



}


#endif // SENTINEL_HPP_INCLUDED
