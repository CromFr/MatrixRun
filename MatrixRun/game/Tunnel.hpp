#ifndef TUNNEL_HPP_INCLUDED
#define TUNNEL_HPP_INCLUDED

#include <deque>

#include "Object.hpp"
#include "../irr/IEmptySceneNode.h"

#include "../base/Ellipse.hpp"

#include "TunnelModule.hpp"


namespace game
{
	/**
	@brief Scene node that contain multiple tunnel modules. The tunnel modules are auto-generated
	**/
	class Tunnel : public Object, public scene::IEmptySceneNode
	{
	public:

		Tunnel(scene::ISceneNode* parent, scene::ISceneManager* mgr);

		~Tunnel();

		/**
		@brief Overrides OnAnimate from IEmptySceneNode
		**/
		void OnAnimate(u32 timeMs);

		/**
		@brief Gets if the given point is in the tunnel
		@param posAbs Absolute position to check
		@param outCheckedTunnel Output param that point to the tunnel module at this position
		@return true if inside the tunnel, else false
		@note always return true if the position is before the tunnel module, and false if after the last
		**/
		bool GetIsInTunnel(const core::vector3df& posAbs, scene::ISceneNode** outCheckedTunnel=0);

		/**
		@brief Implementation of Object
		**/
		virtual enum ObjectType GetType(){return tunnel_border;}



	private:
		base::Ellipse RandomizeEllipse(const base::Ellipse& base);

		deque<TunnelModule*> m_dqTunnelModules;



	};

}


#endif // TUNNEL_HPP_INCLUDED
