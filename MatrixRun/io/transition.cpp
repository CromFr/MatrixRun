
#include "transition.hpp"

/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
mrio::wm::Wiimote3d World3dToWiimote3d(const irr::core::vector3df& World, bool bBorner)
{

    /*if(bBorner)
    {
        if(World.X>WORLD_WIDTH/2)
            World.X=WORLD_WIDTH/2;
        else if(World.X<-WORLD_WIDTH/2)
            World.X=-WORLD_WIDTH/2;

        if(World.Y>WORLD_HEIGHT/2)
            World.Y=WORLD_HEIGHT/2;
        else if(World.Y<-WORLD_HEIGHT/2)
            World.Y=-WORLD_HEIGHT/2;
    }

    return Wiimote3d(SCREEN_WIDTH_MM*World.X/WORLD_WIDTH, World.Z, SCREEN_HEIGHT_MM*World.Y/WORLD_HEIGHT);
    */
    return World3dToWiimote3d(World.X, World.Y, World.Z, bBorner);
}

mrio::wm::Wiimote3d World3dToWiimote3d(float x, float y, float z, bool bBorner)
{
    if(bBorner)
    {
        if(x>WORLD_WIDTH/2)
            x=WORLD_WIDTH/2;
        else if(x<-WORLD_WIDTH/2)
            x=-WORLD_WIDTH/2;

        if(y>WORLD_HEIGHT/2)
            y=WORLD_HEIGHT/2;
        else if(y<-WORLD_HEIGHT/2)
            y=-WORLD_HEIGHT/2;
    }

    return mrio::wm::Wiimote3d(SCREEN_WIDTH_MM*x/WORLD_WIDTH, z, SCREEN_HEIGHT_MM*y/WORLD_HEIGHT);
}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
irr::core::vector3df Wiimote3dToWorld3d(const mrio::wm::Wiimote3d& Wiimote, bool bBorner)
{
	/*
    if(bBorner)
    {
        if(Wiimote.x>SCREEN_WIDTH_MM/2)
            Wiimote.x=SCREEN_WIDTH_MM/2;
        else if(Wiimote.x<-SCREEN_WIDTH_MM/2)
            Wiimote.x=-SCREEN_WIDTH_MM/2;

        if(Wiimote.z>SCREEN_HEIGHT_MM/2)
            Wiimote.z=SCREEN_HEIGHT_MM/2;
        else if(Wiimote.z<-SCREEN_HEIGHT_MM/2)
            Wiimote.z=-SCREEN_HEIGHT_MM/2;
    }

    return irr::core::vector3df(WORLD_WIDTH * Wiimote.x/SCREEN_WIDTH_MM, Wiimote.z, WORLD_HEIGHT * Wiimote.y/SCREEN_HEIGHT_MM);
    */
    return Wiimote3dToWorld3d(Wiimote.x, Wiimote.y, Wiimote.z, bBorner);
}

irr::core::vector3df Wiimote3dToWorld3d(float x, float y, float z, bool bBorner)
{
    if(bBorner)
    {
        if(x>SCREEN_WIDTH_MM/2)
            x=SCREEN_WIDTH_MM/2;
        else if(x<-SCREEN_WIDTH_MM/2)
            x=-SCREEN_WIDTH_MM/2;

        if(z>SCREEN_HEIGHT_MM/2)
            z=SCREEN_HEIGHT_MM/2;
        else if(z<-SCREEN_HEIGHT_MM/2)
            z=-SCREEN_HEIGHT_MM/2;
    }

    return irr::core::vector3df(WORLD_WIDTH*x/SCREEN_WIDTH_MM, WORLD_HEIGHT*z/SCREEN_HEIGHT_MM, y);
}
