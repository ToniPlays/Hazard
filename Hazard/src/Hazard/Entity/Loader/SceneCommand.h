#pragma once

#include "entt.hpp"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Scripting/ScriptCommand.h"

namespace Hazard::ECS {
	class SceneCommand {
	public:

		static void Init();

		//Render component
		template<typename C, typename T>
		static void Render(C& component, T& transform);
		static void RenderScene(glm::mat4 projection, glm::mat4 view, glm::vec4 clearColor);
		//Call when script is added or removed from entity
		static void OnScriptAttached(Entity& entity, ScriptComponent& script);
		static void OnScriptDetached(Entity& entity, ScriptComponent& script);

		//Get current scene from SceneHandler
		static Scene& GetCurrentScene();

	private:
		static Rendering::RenderEngine RenderEngine;
	};
}