#include "World.hpp"

using namespace irr;

namespace game
{

	World* World::m_Instance;
	World* World::GetInstance()
	{
		return m_Instance;
	}

}
