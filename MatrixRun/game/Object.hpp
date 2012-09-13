#ifndef GAME_OBJECT_HPP_INCLUDED
#define GAME_OBJECT_HPP_INCLUDED

#include <iostream>

namespace game
{
	enum ObjectType
	{
		none=0,

		///The borders of the tunnel
		tunnel_border=0b1,

		///The blocks that are fixed to the tunnel (doodads)
		tunnel_block=0b10,

		///Bullet fired by the player/boss
		bullet=0b100,

		///An enemy
		sentinel=0b1000,

		///The ship of the player (Nebuchadnezzar ;)
		ship=0b10000
	};




	class Object
	{
	public:

		virtual enum ObjectType GetType()const=0;
		const char* GetObjectTypeString()
		{
			enum ObjectType type = GetType();
			std::string sType;
			if(type == none) 			sType+="|None";
			if(type == tunnel_border) 	sType+="|Tunnel-Border";
			if(type == tunnel_block) 	sType+="|Tunnel-Block";
			if(type == bullet) 			sType+="|Bullet";
			if(type == sentinel) 		sType+="|Sentinel";
			if(type == ship) 			sType+="|Ship";
			return sType.c_str();
		}
	};


}



#endif // GAME_OBJECT_HPP_INCLUDED
