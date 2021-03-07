#pragma once

#include "hzrpch.h"
#include "ScriptCommand.h"

namespace Hazard::Scripting {

	ScriptEngine* ScriptCommand::scriptEngine;

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

		HZR_CORE_INFO("Adding script component");
	}
	void ScriptCommand::ShutdownScriptEntity(ECS::Entity& entity, std::string& oldComponent)
	{

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
}
