#ifndef GRAVITYANIMATOR_HPP_INCLUDED
#define GRAVITYANIMATOR_HPP_INCLUDED

#include <iostream>
#include <vector>
using namespace std;


#include <irrlicht.h>
using namespace irr;





class GravityAnimator : public scene::ISceneNodeAnimator
{

    public:
    GravityAnimator(float fMasseKg, float fGravity=98, float fAirFriction=0, core::vector3df OptForce=core::vector3df(0,0,0));

    ///Va suivre un node en appliquant une force dirig�e vers celui ci, dont la norme est �gale � fIntensity
    void FollowNode(scene::ISceneNode* Node, float fIntensity, core::vector3df m_vFollowOffset=core::vector3df(0));

    ///Nettoye tous les follow pr�c�dament lanc�s
    void StopFollow(){m_nodeFollowedNode=0;}

    ///Change instantan�ment la vitesse du node
    void SetSpeed(core::vector3df Speed){m_Speed = Speed;}

    ///Change la valeur de la force appliqu�e au node
    void SetOptForce(core::vector3df Force){m_OptForce = Force;}

    ///Change les frottements fluides de l'air appliqu�s au node
    void SetAirFriction(float fCoef){m_fAirFriction = fCoef;}

    ///Change la gravit� sp�cifique appliqu�e
    void SetGravity(float fGravity){m_fGravity = fGravity;}


    ///Override
    void animateNode(scene::ISceneNode *node, u32 timeMs);
    scene::ISceneNodeAnimator* createClone(scene::ISceneNode*, scene::ISceneManager*);



    private:
    float m_fGravity;

    scene::ISceneNode* m_nodeFollowedNode;
    float m_fFollowIntensity;
    core::vector3df m_vFollowOffset;


    core::vector3df m_DesiredSpeed;
    core::vector3df m_Speed;


    core::vector3df m_OptForce;
    float m_fAirFriction;
    float m_fMass;

    long m_nLastCalculDate;
// TODO (Administrateur#1#): v�rifier long/u32


};


// dV = dt(-g*y + m_OptForce/m_fMass - m_fAirFriction/m_fMass * m_Speed);

#endif // GRAVITYANIMATOR_HPP_INCLUDED
