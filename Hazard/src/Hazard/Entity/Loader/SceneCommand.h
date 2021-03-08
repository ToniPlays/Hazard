#pragma once

#include "entt.hpp"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Scripting/ScriptCommand.h"

namespace Hazard::ECS {
	class SceneCommand {
	public:

		static void Init();

		template<typename C, typename T>
		static void Render(C& component, T& transform);
		static void OnScriptAttached(Entity& entity, ScriptComponent& script);
		static void OnScriptDetached(Entity& entity, ScriptComponent& script);

		static Scene& GetCurrentScene();

	private:
		static Rendering::RenderEngine RenderEngine;
	};
}