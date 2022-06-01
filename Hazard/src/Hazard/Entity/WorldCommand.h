#pragma once

#include "WorldHandler.h"
#include "Entity.h"
#include "entt.hpp"

namespace Hazard::ECS {

	class WorldCommand {
	public:
		static void Init();
		static void WorldRuntimeBegin();
		static void WorldRuntimeEnd();

		static Entity GetEntity(uint32_t id);
		static Ref<World> GetCurrentWorld();

		template<typename T>
		static void OnScriptAttached(Entity& entity, T& script);
		template<typename T>
		static void OnScriptDetached(Entity& entity, T& script);
		static void UpdatePhysics();
		static void RenderWorld();

	private:
		inline static WorldHandler* m_Handler;
	};
}