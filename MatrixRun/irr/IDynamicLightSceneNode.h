#ifndef IDYNAMICLIGHTSCENENODE_H_INCLUDED
#define IDYNAMICLIGHTSCENENODE_H_INCLUDED

#include <irrlicht.h>
#include "IEmptySceneNode.h"
using namespace irr;

namespace irr
{
	namespace scene
	{
		class IDynamicLightSceneNode : public IEmptySceneNode
		{
		public:
			IDynamicLightSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, const core::vector3df& position, float fRadius, const video::SColor& color)
						: IEmptySceneNode(parent, mgr, id, position)
			{
				m_light = mgr->addLightSceneNode(parent, position, color, fRadius);
				m_color = &(m_light->getLightData().DiffuseColor);
				//m_light->enableCastShadow(false);

				m_nStartDate=-1;
				m_nEndDate=-1;
			}
			~IDynamicLightSceneNode()
			{
				m_light->remove();
			}


			void FadeToColor(const video::SColor& color, float fTimeS)
			{
				m_nStartDate=0;
				m_nEndDate = fTimeS*1000;
				m_clrStartingColor = *m_color;
				m_clrEndingColor = color;
			}

			void OnAnimate(u32 timeMs)
			{
				if(m_nStartDate==0)
				{
					m_nStartDate += timeMs;
					m_nEndDate += timeMs;
				}

				if(m_nEndDate>=timeMs)
				{
					float fRatio = (timeMs-m_nStartDate)/(float)(m_nEndDate-m_nStartDate);

					*m_color = m_clrStartingColor.getInterpolated(m_clrEndingColor, 1-fRatio);

				}
			}


		private:
			ILightSceneNode* m_light;
			video::SColorf* m_color;

			u32 m_nStartDate;
			u32 m_nEndDate;
			video::SColorf m_clrStartingColor;
			video::SColorf m_clrEndingColor;

		};

	}
}


#endif // IDYNAMICLIGHTSCENENODE_H_INCLUDED
