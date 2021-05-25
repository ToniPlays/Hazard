#pragma once

#include "entt.hpp"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Scripting/ScriptCommand.h"
#include "Hazard/Rendering/Camera.h"

namespace Hazard::ECS {

	class WorldCommand {
	public:
		static Entity GetEntity(uint32_t id);
		static World& GetCurrentWorld();

		template<typename C, typename T>
		static void Render(C& component, T& transform);
		static void RenderScene(Rendering::Camera camera);

		template<typename T>
		static void OnScriptAttached(Entity& entity, T& script);
		template<typename T>
		static void OnScriptDetached(Entity& entity, T& script);

	};
}