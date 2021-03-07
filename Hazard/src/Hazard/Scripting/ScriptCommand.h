#pragma once

#include "Hazard/Entity/Entity.h"
#include "Hazard/Entity/Component.h"

#include "ScriptEngine.h"

namespace Hazard::Scripting {
	class ScriptCommand {
	public:
		static void Init();
		static void InitScripEntity(ECS::Entity& entity, ECS::ScriptComponent& component);
		static void ShutdownScriptEntity(ECS::Entity& entity, std::string& oldComponent);
		static void RemoveScriptableEntity(ECS::Entity& entity, ECS::ScriptComponent& component);
		static bool ModuleExists(std::string& module);

		static EntityInstanceData& GetInstanceData(uint32_t entity);
		static void DoStep();

		static void InitAllEntities();
	private:
		static ScriptEngine* scriptEngine;
	};
}