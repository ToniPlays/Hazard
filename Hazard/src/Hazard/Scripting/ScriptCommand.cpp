#pragma once

#include "hzrpch.h"
#include "ScriptCommand.h"
#include "Hazard/Entity/Loader/SceneHandler.h"

namespace Hazard::Scripting {

	ScriptEngine* ScriptCommand::scriptEngine;
	void(*ScriptCommand::debugCallback)(Severity, std::string);

	void ScriptCommand::Init()
	{
		scriptEngine = &Application::GetModule<ScriptEngine>();
	}
	void ScriptCommand::InitScripEntity(ECS::Entity& entity, ECS::ScriptComponent& component)
	{
		ECS::Scene& scene = entity.GetScene();
		uint32_t UUID = entity;
		auto& moduleName = component.moduleName;
		if (moduleName == "Hazard_NULL") return;
		if (!scriptEngine->ModuleExists(moduleName)) return;
		scriptEngine->RegisterScripEntity(moduleName, UUID);
	}
	void ScriptCommand::ShutdownScriptEntity(ECS::Entity& entity, std::string& oldComponent)
	{
		scriptEngine->RemoveScriptEntity(oldComponent, entity);
	}
	void ScriptCommand::RemoveScriptableEntity(ECS::Entity& entity, ECS::ScriptComponent& component)
	{
		HZR_CORE_INFO("Removing scriptable entity");
	}
	bool ScriptCommand::ModuleExists(std::string& module)
	{
		return scriptEngine->ModuleExists(module);
	}
	EntityInstanceData& ScriptCommand::GetInstanceData(uint32_t entity) {
		return scriptEngine->GetInstanceData(entity);
	}
	void ScriptCommand::DoStep()
	{
		using namespace ECS;
		Scene& current = Application::GetModule<SceneHandler>().GetCurrentScene();
		auto view = current.GetSceneRegistry().view<ScriptComponent>();

		for (auto entity : view) {
			Entity e{ entity, &current };
			auto& c = e.GetComponent<ScriptComponent>();
			if (ModuleExists(c.moduleName)) {
				scriptEngine->InstantiateScriptEntity(e, c.moduleName);
			}
		}
	}

	void ScriptCommand::InitAllEntities()
	{
		using namespace ECS;
		Scene& current = Application::GetModule<SceneHandler>().GetCurrentScene();
		current.GetSceneRegistry().each([&](auto entityID) {
			Entity entity{ entityID, &current };


			if (entity.HasComponent<ScriptComponent>()) {
				HZR_CORE_INFO("Initializing {0}", entity.GetComponent<TagComponent>().tag);
				InitScripEntity(entity, entity.GetComponent<ScriptComponent>());
			}
		});
	}
	void ScriptCommand::SendDebugMessage(Severity severity, std::string message)
	{
		debugCallback(severity, message);
	}
}
