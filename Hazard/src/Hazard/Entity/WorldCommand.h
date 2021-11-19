#pragma once

#include "Hazard/Scripting/ScriptCommand.h"
#include "WorldHandler.h"
#include "entt.hpp"

namespace Hazard::ECS {

	class WorldCommand {
	public:
		static void Init();
		static Entity GetEntity(uint32_t id);
		static Ref<World> GetCurrentWorld();

		template<typename C, typename T>
		static void Render(C& component, T& transform);

		template<typename T>
		static void OnScriptAttached(Entity& entity, T& script);
		template<typename T>
		static void OnScriptDetached(Entity& entity, T& script);

	private:
		static void ProcessWorld();
		inline static WorldHandler* m_Handler;
	};
}