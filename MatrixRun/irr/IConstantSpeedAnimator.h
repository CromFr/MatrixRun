#ifndef IFOLLOWLINEANIMATOR_H_INCLUDED
#define IFOLLOWLINEANIMATOR_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

namespace irr
{
	namespace scene
	{

		class IConstantSpeedAnimator : public scene::ISceneNodeAnimator
		{

			public:
			IConstantSpeedAnimator(core::vector3df direction, float fSpeedS)
			{
				m_vectDirection.set(direction.normalize());
				m_fSpeedS = fSpeedS;
			}


			void animateNode(scene::ISceneNode *node, u32 timeMs)
			{
				core::vector3df pos = node->getPosition();
				node->setPosition(pos+m_vectDirection*m_fSpeedS*(timeMs/1000.0));
			}
			scene::ISceneNodeAnimator* createClone(scene::ISceneNode*, scene::ISceneManager*)
			{
				return 0;//ISceneNodeAnimator(m_vectDirection, m_fSpeedS);
			}



			private:
			core::vector3df m_vectDirection;
			float m_fSpeedS;


		};
	}
}

#endif // IFOLLOWLINEANIMATOR_H_INCLUDED
