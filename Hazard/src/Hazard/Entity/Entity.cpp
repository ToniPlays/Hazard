#pragma once

#include <hzrpch.h>
#include "Entity.h"

namespace Hazard::ECS {

	Entity::Entity(entt::entity handle, World* world) : m_Handle(handle), m_World(world) {}
}