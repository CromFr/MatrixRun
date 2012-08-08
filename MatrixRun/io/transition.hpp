#ifndef TRANSITION_HPP_INCLUDED
#define TRANSITION_HPP_INCLUDED

#include <irrlicht.h>
#include "WiimoteHandler.hpp"


#define SCREEN_WIDTH_MM 520
#define SCREEN_HEIGHT_MM 300

#define WORLD_WIDTH 300
#define WORLD_HEIGHT 300


Wiimote3d World3dToWiimote3d(irr::core::vector3df World, bool bBorner=false);
Wiimote3d World3dToWiimote3d(float x, float y, float z, bool bBorner=false);

irr::core::vector3df Wiimote3dToWorld3d(Wiimote3d Wiimote, bool bBorner=false);
irr::core::vector3df Wiimote3dToWorld3d(float x, float y, float z, bool bBorner=false);

#endif // TRANSITION_HPP_INCLUDED
