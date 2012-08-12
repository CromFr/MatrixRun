#ifndef IEMPTYSCENENODE_H_INCLUDED
#define IEMPTYSCENENODE_H_INCLUDED

#include <irrlicht.h>

namespace irr
{
	namespace scene
	{

		class IEmptySceneNode : public ISceneNode
		{
		public:

			IEmptySceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, const core::vector3df& position)
						: ISceneNode(parent, mgr, id, position), box(0,0,0,0,0,0)
			{

			}

			virtual void render()
			{

			}

			virtual const core::aabbox3d<f32>& getBoundingBox() const
			{
				return box;
			}

		private:
			core::aabbox3d<f32> box;

		};

	}
}

#endif // IEMPTYSCENENODE_H_INCLUDED
