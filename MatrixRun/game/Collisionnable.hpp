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
	class Collisionnable : public scene::IEmptySceneNode
	{
	public:
		/**
		@brief Unique constructor
		@param funcCallback Boost function to call in case of collision
		@param nCollisionFlags ID of the scene nodes that can have collision with this object
		**/
		Collisionnable(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
				boost::function<void(scene::ISceneNode*, core::triangle3df&, core::vector3df&)> funcCallback, int nCollisionFlags=0);

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
		void AddCollisionLine(const core::line3df& line)
		{
			m_CollPoints.push_back(new coll::CollLine(this, line));
		}

		/**
		@brief Adds a point that can generate collisions
		**/
		void AddCollisionPoint(const core::vector3df& point)
		{
			m_CollPoints.push_back(new coll::CollPoint(this, point));
		}


		/**
		@brief Removes all collision points/lines
		**/
		void ClearCollisions();



	protected:
		void OnAnimate(u32 timeMs);

		scene::ISceneCollisionManager* m_CollM;

		int m_nCollisionFlags;

	private:
		vector<coll::CollGeometry*> m_CollPoints;
		boost::function<void(scene::ISceneNode*, core::triangle3df&, core::vector3df&)> m_funcCallback;


	};






}

#endif // COLLISIONNABLE_HPP_INCLUDED
