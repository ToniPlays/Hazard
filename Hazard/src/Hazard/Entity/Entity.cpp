#pragma once

#include <hzrpch.h>
#include "Entity.h"

namespace Hazard::ECS {
	Entity::Entity(entt::entity handle, Scene* scene) : handle(handle), scene(scene)
	{

	}
}