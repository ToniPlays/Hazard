#pragma once

#include "Hazard/Entity/Entity.h"
#include "Hazard/Entity/Component.h"
#include "Hazard/Entity/WorldCommand.h"
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

		static void InitAllEntities();

		static void InitEntity(ECS::Entity entity, ECS::ScriptComponent& component);
		static void ClearEntity(ECS::Entity entity, ECS::ScriptComponent& component);
		static void InitEntity(ECS::Entity entity, ECS::VisualScriptComponent& component);
		static void ClearEntity(ECS::Entity entity, ECS::VisualScriptComponent& component);

		static bool ModuleExists(ScriptType type, const char* name) { 
			return s_manager->ModuleExists(type, name);
		};
		static std::unordered_map<std::string, PublicField*> GetPublicFields(ScriptType type, uint32_t entity, const std::string& moduleName) {
			return s_manager->GetPublicFields(type, entity, moduleName);
		}
		template<typename T>
		static T& EntityGetComponent(uint32_t entityID) {
			return ECS::WorldCommand::GetEntity(entityID).GetComponent<T>();
		}
		template<typename T>
		static bool EntityHasComponent(uint32_t entityID) 
		{
			return ECS::WorldCommand::GetEntity(entityID).HasComponent<T>();
		}
		template<typename T>
		static void EntityAddComponent(uint32_t entityID) {
			ECS::WorldCommand::GetEntity(entityID).AddComponent<T>();
		}

		static void SendDebugMessage(Severity severity, std::string message);
		static void SetDebugCallback(void(*callback)(Severity, std::string)) { debugCallback = callback; };

	private:
		static void(*debugCallback)(Severity, std::string);
		static ScriptEngineManager* s_manager;
	};
}