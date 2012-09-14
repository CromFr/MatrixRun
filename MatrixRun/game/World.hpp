#ifndef GAMEMANAGER_HPP_INCLUDED
#define GAMEMANAGER_HPP_INCLUDED

#include <irrlicht.h>
#include "../irr/IConstantSpeedAnimator.h"
#include "../irr/IEmptySceneNode.h"

#include "Tunnel.hpp"

namespace game
{
	/**
	@brief Scene node containing all moving entities, meaning the tunnel, sentinels, ... without the camera and player
	@note World is moving to the camera at 400 units/sec
	**/
	class World : public irr::scene::IEmptySceneNode
	{
	public:
		static World* GetInstance();


		World(irr::scene::ISceneManager* mgr)
			: IEmptySceneNode(mgr->getRootSceneNode(), mgr, -1, core::vector3df(0,0,0)),
			m_tunnel(this, mgr),
			m_animator(irr::core::vector3df(0,0,1), -400)
		{
			addAnimator(&m_animator);

			m_Instance = this;
		}

		Tunnel* GetTunnel(){return &m_tunnel;}



	private:
		Tunnel m_tunnel;

		irr::scene::IConstantSpeedAnimator m_animator;

		static World* m_Instance;



	};
}


#endif // GAMEMANAGER_HPP_INCLUDED
