
#include <hzrpch.h>
#include "Entity.h"

namespace Hazard::ECS 
{
	Entity::Entity(entt::entity handle, World* world) : m_Handle(handle), m_World(world) {}
	bool Entity::IsValid() 
	{
		if (m_World == nullptr)
			return false;

		return m_World->GetWorldRegistry().valid(m_Handle);
	}
}
