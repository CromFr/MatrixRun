#ifndef GAMEMANAGER_HPP_INCLUDED
#define GAMEMANAGER_HPP_INCLUDED

#include <irrlicht.h>
#include "../irr/IConstantSpeedAnimator.h"
#include "../irr/IEmptySceneNode.h"

#include "Tunnel.hpp"

namespace game
{
	/**
	Node contenant toutes les entités du jeu, sauf les entitées "fixes" comme le joueur & la caméra
	World avance vers la caméra à 400u/s
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
