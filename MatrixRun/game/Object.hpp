#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include "../irr/IEmptySceneNode.h"
using namespace irr;

namespace game
{
	enum ObjectType
	{
		none=0,

		///The borders of the tunnel
		border=0b1,

		///The blocks that are fixed to the tunnel (doodads)
		block=0b10,

		///Bullet fired by the player/boss
		bullet=0b100,

		///An enemy
		sentinel=0b1000,

		///The ship of the player (Nebuchadnezzar ;)
		ship=0b10000
	};

	const char* GetObjectTypeString(enum ObjectType type)
	{
		string sType;
		if(type == none) 		sType+="|None";
		if(type == border) 		sType+="|Border";
		if(type == block) 		sType+="|Block";
		if(type == bullet) 		sType+="|Bullet";
		if(type == sentinel) 	sType+="|Sentinel";
		if(type == ship) 		sType+="|Ship";
		return sType.c_str();
	}



	class Object : public scene::IEmptySceneNode
	{
	public:
		Object(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, const core::vector3df& position)
						: IEmptySceneNode(parent, mgr, id, position)
		{

		}

		virtual enum ObjectType GetType()=0;
	};


}



#endif // OBJECT_HPP_INCLUDED
