#pragma once

#include "Hazard/Scripting/ScriptCommand.h"
#include "entt.hpp"

namespace Hazard::ECS {

	class WorldCommand {
	public:
		static Entity GetEntity(uint32_t id);
		static World& GetCurrentWorld();

		template<typename C, typename T>
		static void Render(C& component, T& transform);

		template<typename T>
		static void OnScriptAttached(Entity& entity, T& script);
		template<typename T>
		static void OnScriptDetached(Entity& entity, T& script);

	};
}