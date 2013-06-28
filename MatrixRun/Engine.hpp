#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include <irrlicht.h>
#include "io/IOManager.hpp"

namespace Engine
{

	extern mrio::IOManager* IOManager;

	extern irr::IrrlichtDevice* IrrDevice;

	extern irr::video::IVideoDriver* IrrDriver;

	extern irr::scene::ISceneManager* IrrSceneManager;

	extern irr::scene::ISceneCollisionManager* IrrCollisionManager;



}


#endif // ENGINE_HPP_INCLUDED
