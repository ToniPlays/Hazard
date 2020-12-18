#pragma once

#include <hzrpch.h>
#include "Entity.h"

namespace Hazard {

	namespace ECS {

		Entity::Entity(entt::entity entityHandle, Scene* _scene) : handle(entityHandle), scene(_scene) {
			
		}
	}
}

