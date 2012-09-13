#ifndef COLLISIONNABLE_HPP_INCLUDED
#define COLLISIONNABLE_HPP_INCLUDED

#include "../irr/IEmptySceneNode.h"

#include "CollGeometry.hpp"

#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace game
{
	/**
	@brief Scene node that contain collision points and execute a callback in case of collision
	**/
	class Collisionnable : public irr::scene::IEmptySceneNode
	{
	public:
		/**
		@brief Unique constructor
		@param funcCallback Boost function to call in case of collision
		@param nCollisionFlags ID of the scene nodes that can have collision with this object
		**/
		Collisionnable(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, s32 id,
				boost::function<void(irr::scene::ISceneNode*, irr::core::triangle3df&, irr::core::vector3df&)> funcCallback, int nCollisionFlags=0);

		~Collisionnable();

		/**
		@note The collision flags are the ID of the objects that can generate collisions
		**/
		void SetCollisionFlags(int nFlags){m_nCollisionFlags = nFlags;}

		/**
		@note The collision flags are the ID of the objects that can generate collisions
		**/
		int GetCollisionFlags()const{return m_nCollisionFlags;}

		/**
		@brief Adds a line that can generate collisions
		**/
		void AddCollisionLine(const irr::core::line3df& line)
		{
			m_CollPoints.push_back(new coll::CollLine(this, line));
		}

		/**
		@brief Adds a point that can generate collisions
		**/
		void AddCollisionPoint(const irr::core::vector3df& point)
		{
			m_CollPoints.push_back(new coll::CollPoint(this, point));
		}


		/**
		@brief Removes all collision points/lines
		**/
		void ClearCollisions();



	protected:
		void OnAnimate(irr::u32 timeMs);

		irr::scene::ISceneCollisionManager* m_CollM;

		int m_nCollisionFlags;

	private:
		std::vector<coll::CollGeometry*> m_CollPoints;
		boost::function<void(irr::scene::ISceneNode*, irr::core::triangle3df&, irr::core::vector3df&)> m_funcCallback;


	};






}

#endif // COLLISIONNABLE_HPP_INCLUDED
