#pragma once

#include "entt.hpp"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Scripting/ScriptCommand.h"

#include "Hazard/Rendering/Camera.h"

namespace Hazard::ECS {
	class SceneCommand {
	public:
		//Render component
		template<typename C, typename T>
		static void Render(C& component, T& transform);
		static void RenderScene(Rendering::Camera camera);
		//Call when script is added or removed from entity
		static void OnScriptAttached(Entity& entity, ScriptComponent& script);
		static void OnScriptDetached(Entity& entity, ScriptComponent& script);
		static Entity GetEntity(uint32_t id);

		//Get current scene from SceneHandler
		static Scene& GetCurrentScene();
	};
}