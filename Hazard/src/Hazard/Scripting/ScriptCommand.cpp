#pragma once

#include "hzrpch.h"
#include "ScriptCommand.h"
#include "Hazard/Entity/Loader/SceneHandler.h"

namespace Hazard::Scripting {

	void(*ScriptCommand::debugCallback)(Severity, std::string);
	ScriptEngineManager* ScriptCommand::manager;

	void ScriptCommand::Init()
	{
		manager = &Application::GetModule<ScriptEngineManager>();
	}
	void ScriptCommand::OnBeginRuntime()
	{
		manager->BeginRuntime();
	}
	void ScriptCommand::OnEndRuntime()
	{
		manager->EndRuntime();
	}
	void ScriptCommand::InitEntity(ECS::Entity entity, ECS::ScriptComponent& component)
	{
		manager->InitEntity((uint32_t)entity, component.moduleName, ScriptType::CSharpScript);
	}
	void ScriptCommand::ClearEntity(ECS::Entity entity, ECS::ScriptComponent& component)
	{
		manager->ClearEntity((uint32_t)entity, component.moduleName, ScriptType::CSharpScript);
	}
	void ScriptCommand::InitEntity(ECS::Entity entity, ECS::VisualScriptComponent& component)
	{
		manager->InitEntity((uint32_t)entity, component.filename, ScriptType::CSharpScript);
	}
	void ScriptCommand::ClearEntity(ECS::Entity entity, ECS::VisualScriptComponent& component)
	{
		manager->ClearEntity((uint32_t)entity, component.filename, ScriptType::CSharpScript);
	}
	ScriptData ScriptCommand::GetData(ScriptType type, ECS::Entity entity, std::string moduleName) {
		return manager->GetData(type, entity, moduleName);
	}
	void ScriptCommand::SendDebugMessage(Severity severity, std::string message)
	{
		debugCallback(severity, message);
	}
}
