#ifndef TUNNEL_HPP_INCLUDED
#define TUNNEL_HPP_INCLUDED

#include <deque>

#include "Object.hpp"
#include "../irr/IEmptySceneNode.h"

#include "../base/Ellipse.hpp"

#include "TunnelModule.hpp"

//@todo : remove this, testing only
#include "Sentinel.hpp"


namespace game
{
	/**
	@brief Scene node that contain multiple tunnel modules. The tunnel modules are auto-generated
	@note moves with the world, and automatically moves back %400 z
	**/
	class Tunnel : public Object, public irr::scene::IEmptySceneNode
	{
	public:

		Tunnel(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr);

		~Tunnel();

		/**
		@brief Overrides OnAnimate from IEmptySceneNode
		**/
		void OnAnimate(irr::u32 timeMs);

		/**
		@brief Gets if the given point is in the tunnel
		@param posAbs Absolute position to check
		@param outCheckedTunnel Output param that point to the tunnel module at this position
		@return true if inside the tunnel, else false
		@note always return true if the position is before the tunnel module, and false if after the last
		**/
		bool GetIsInTunnel(const irr::core::vector3df& posAbs, irr::scene::ISceneNode** outCheckedTunnel=0, irr::core::vector3df* outCollPosAbs=0)const;

		/**
		@brief Implementation of Object
		**/
		virtual enum ObjectType GetType()const{return tunnel_border;}



	private:
		base::Ellipse RandomizeEllipse(const base::Ellipse& base);

		std::deque<TunnelModule*> m_dqTunnelModules;



	};

}


#endif // TUNNEL_HPP_INCLUDED
