#pragma once

#include "Hazard/Entity/Entity.h"
#include "Hazard/Entity/Component.h"

#include "ScriptEngine.h"

namespace Hazard::Scripting {

	enum class Severity {
		Debug,
		Trace,
		Info,
		Warning,
		Error,
		Critical
	};

	class ScriptCommand {
		friend class ScriptEngine;
	public:
		static void Init(ScriptEngine* engine);
		static void OnBeginRuntime();
		static void OnEndRuntime();
		static void InitScripEntity(ECS::Entity& entity, ECS::ScriptComponent& component);
		static void ShutdownScriptEntity(ECS::Entity& entity, std::string& oldComponent);
		static void RemoveScriptableEntity(ECS::Entity& entity, ECS::ScriptComponent& component);
		static bool ModuleExists(std::string& module);

		static EntityInstanceData& GetInstanceData(uint32_t entity);
		static void DoStep();
		static void SetDebugCallback(void(*fn)(Severity, std::string)) { debugCallback = fn; }
		static void InitAllEntities();
		static void SendDebugMessage(Severity severity, std::string message);

		static void ReloadRuntimeAssembly() { scriptEngine->ReloadRuntimeAssembly(); }
	private:
		static ScriptEngine* scriptEngine;
		static void(*debugCallback)(Severity, std::string);
	};
}