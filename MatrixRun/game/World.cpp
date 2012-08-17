#include "World.hpp"

namespace game
{

	World* World::m_Instance;
	World* World::GetInstance()
	{
		return m_Instance;
	}

}
