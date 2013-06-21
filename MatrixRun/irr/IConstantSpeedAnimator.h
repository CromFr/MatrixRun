#ifndef IFOLLOWLINEANIMATOR_H_INCLUDED
#define IFOLLOWLINEANIMATOR_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

namespace irr
{
	namespace scene
	{

		/**
		@brief Animator tha moves the node on a given direction at a constant speed
		**/
		class IConstantSpeedAnimator : public scene::ISceneNodeAnimator
		{
			public:
			IConstantSpeedAnimator(core::vector3df direction, f32 fSpeedS)
			{
				m_vectDirection.set(direction.normalize());
				m_fSpeedS = fSpeedS;
				m_LastCalculDateMS = 0;
			}


			void animateNode(scene::ISceneNode *node, u32 timeMs)
			{
				if(m_LastCalculDateMS==0)
				{
					m_LastCalculDateMS = timeMs;
					return;
				}

				core::vector3df pos = node->getPosition();
				node->setPosition(pos+m_vectDirection*m_fSpeedS*((timeMs-m_LastCalculDateMS)/1000.0));

				m_LastCalculDateMS = timeMs;
			}
			scene::ISceneNodeAnimator* createClone(scene::ISceneNode*, scene::ISceneManager*)
			{
				return 0;
			}



			private:
			core::vector3df m_vectDirection;
			f32 m_fSpeedS;

			u32 m_LastCalculDateMS;


		};
	}
}

#endif // IFOLLOWLINEANIMATOR_H_INCLUDED
