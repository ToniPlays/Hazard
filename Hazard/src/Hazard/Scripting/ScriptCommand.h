#pragma once

#include "Hazard/Entity/Entity.h"
#include "Hazard/Entity/Component.h"
#include "Hazard/Entity/SceneCommand.h"
#include "ScriptEngineManager.h"
#include "Hazard/Entity/Entity.h"

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

	public:
		static void Init();
		static void OnBeginRuntime();
		static void OnEndRuntime();

		static void InitEntity(ECS::Entity entity, ECS::ScriptComponent& component);
		static void ClearEntity(ECS::Entity entity, ECS::ScriptComponent& component);
		static void InitEntity(ECS::Entity entity, ECS::VisualScriptComponent& component);
		static void ClearEntity(ECS::Entity entity, ECS::VisualScriptComponent& component);
		static ScriptData GetData(ScriptType type, ECS::Entity entity, std::string moduleName);

		static bool ModuleExists(ScriptType type, const char* name) { 
			return manager->ModuleExists(type, name); 
		};
		template<typename T>
		static T& EntityGetComponent(uint32_t entityID) {
			return ECS::SceneCommand::GetEntity(entityID).GetComponent<T>();
		}
		template<typename T>
		static bool EntityHasComponent(uint32_t entityID) {
			return ECS::SceneCommand::GetEntity(entityID).HasComponent<T>();
		}
		template<typename T>
		static void EntityAddComponent(uint32_t entityID) {
			ECS::SceneCommand::GetEntity(entityID).AddComponent<T>();
		}

		static void SendDebugMessage(Severity severity, std::string message);
		static void SetDebugCallback(void(*callback)(Severity, std::string)) { debugCallback = callback; };
	private:
		static void(*debugCallback)(Severity, std::string);

		static ScriptEngineManager* manager;
	};
}