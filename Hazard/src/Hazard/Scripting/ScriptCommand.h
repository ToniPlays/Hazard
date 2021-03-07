#pragma once

#include "Hazard/Entity/Entity.h"
#include "Hazard/Entity/Component.h"

#include "ScriptEngine.h"

namespace Hazard::Scripting {
	class ScriptCommand {
	public:
		static void Init();
		static void InitScriptableEntity(ECS::Entity& entity, ECS::ScriptComponent& component);
		static void RemoveScriptableEntity(ECS::Entity& entity, ECS::ScriptComponent& component);
		static bool ModuleExists(std::string& module);
	private:
		static ScriptEngine* scriptEngine;
	};
}