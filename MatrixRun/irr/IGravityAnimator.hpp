#ifndef GRAVITYANIMATOR_HPP_INCLUDED
#define GRAVITYANIMATOR_HPP_INCLUDED

#include <iostream>
#include <vector>


#include <irrlicht.h>

namespace irr
{
	namespace scene
	{
		/**
		@brief Animator to make realistic movements/gravity
		**/
		class IGravityAnimator : public ISceneNodeAnimator
		{

			public:
			IGravityAnimator(float fMasseKg, float fGravity=98, float fAirFriction=0, irr::core::vector3df OptForce=irr::core::vector3df(0,0,0));

			///Va suivre un node en appliquant une force dirigée vers celui ci, dont la norme est égale à fIntensity
			void FollowNode(irr::scene::ISceneNode* Node, float fIntensity, irr::core::vector3df m_vFollowOffset=irr::core::vector3df(0));

			///Nettoye tous les follow précédament lancés
			void StopFollow(){m_nodeFollowedNode=0;}

			///Change instantanément la vitesse du node
			void SetSpeed(irr::core::vector3df Speed){m_Speed = Speed;}

			///Change la valeur de la force appliquée au node
			void SetOptForce(core::vector3df Force){m_OptForce = Force;}

			///Change les frottements fluides de l'air appliqués au node
			void SetAirFriction(float fCoef){m_fAirFriction = fCoef;}

			///Change la gravité spécifique appliquée
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
		// TODO (Administrateur#1#): vérifier long/u32


		};
	}
}

// dV = dt(-g*y + m_OptForce/m_fMass - m_fAirFriction/m_fMass * m_Speed);

#endif // GRAVITYANIMATOR_HPP_INCLUDED
