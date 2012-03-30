#include "GravityAnimator.hpp"

/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
GravityAnimator::GravityAnimator(float fMasseKg, float fGravity, float fAirFriction, core::vector3df OptForce)
{
    m_nLastCalculDate = -1;
    m_fMass = fMasseKg;
    m_fAirFriction = fAirFriction;
    m_OptForce = OptForce;


    m_fGravity = fGravity;

    m_nodeFollowedNode=0;
    m_fFollowIntensity = 0.0;
}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void GravityAnimator::FollowNode(scene::ISceneNode* Node, float fIntensity, float fFollowAbsorption)
{
    m_nodeFollowedNode = Node;
    m_fFollowIntensity = fIntensity;
    m_fFollowAbsorption = fFollowAbsorption;
}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void GravityAnimator::animateNode(scene::ISceneNode *node, u32 timeMs)
{
    if(m_nLastCalculDate==-1)
    {
        m_nLastCalculDate = timeMs;
        return;
    }

    // dV = dt(-g*y + m_OptForce/m_fMass - m_fAirFriction/m_fMass * m_Speed);
    //    = dt(vGravity+vOptForce+vAirFriction)

    core::vector3df vAccel(0);

    //Gravité
    if(m_fGravity!=0)       vAccel+=core::vector3df(0, -m_fGravity, 0);

    //Frotements fluide de l'air
    if(m_fAirFriction!=0)   vAccel+=core::vector3df(-m_Speed*(m_fAirFriction/m_fMass));

    //Force optionnelle
                            vAccel+=core::vector3df(m_OptForce/m_fMass);

    //Force liée au follow
    if(m_nodeFollowedNode!=0)
    {
        // TODO (Administrateur#1#): réduire intensité quand proche du point
        core::vector3df vDirection((m_nodeFollowedNode->getAbsolutePosition()- node->getAbsolutePosition()));
        vAccel+=core::vector3df(  vDirection * (m_fFollowIntensity/m_fMass)-m_fFollowAbsorption/m_fMass  );
    }



    float fdt = (timeMs-m_nLastCalculDate)/1000.0;

    m_Speed += fdt*(vAccel);

    node->setPosition(node->getPosition()+m_Speed*fdt);

    //std::cout<<"t="<<timeMs/1000.0<<"\tDt="<<((timeMs-m_nLastCalculDate)/1000.0)<<" \t|VIT:  x="<<m_Speed.X<<"\ty="<<m_Speed.Y<<"\tz="<<m_Speed.Z<<"\t|POS:  x="<<node->getPosition().X<<"\ty="<<node->getPosition().Y<<"\tz="<<node->getPosition().Z<<std::endl;

    m_nLastCalculDate = timeMs;
}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
scene::ISceneNodeAnimator* GravityAnimator::createClone(scene::ISceneNode*, scene::ISceneManager*)
{
    GravityAnimator* newAnimator = new GravityAnimator(m_fMass, m_fGravity, m_fAirFriction, m_OptForce);
    newAnimator->m_nodeFollowedNode = m_nodeFollowedNode;
    newAnimator->m_fFollowIntensity = m_fFollowIntensity;
    return newAnimator;
}
