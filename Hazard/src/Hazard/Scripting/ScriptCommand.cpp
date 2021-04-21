#pragma once

#include "hzrpch.h"
#include "ScriptCommand.h"
#include "Hazard/Entity/Component.h"
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
		using namespace ECS;
		World& current = Application::GetModule<SceneHandler>().GetCurrentWorld();
		auto view = current.GetWorldRegistry().view<ScriptComponent>();

		for (auto entity : view) {
			Entity e { entity, &current };
			if (!e.IsVisible()) continue;

			auto& c = e.GetComponent<ScriptComponent>();
			if (ModuleExists(ScriptType::CSharpScript, c.moduleName.c_str())) {
				manager->Instantiate(ScriptType::CSharpScript, e, c.moduleName);
			}
		}
	}
	void ScriptCommand::OnEndRuntime()
	{

	}
	void ScriptCommand::InitAllEntities()
	{	
		auto& scene = ECS::SceneCommand::GetCurrentWorld();
		for (auto [id, component] : scene.FindEntitiesWith<ECS::ScriptComponent>()) {
			if (manager->ModuleExists(ScriptType::CSharpScript, component.moduleName.c_str()));
				manager->InitEntity(ScriptType::CSharpScript, id, component.moduleName);
		}
	}
	void ScriptCommand::InitEntity(ECS::Entity entity, ECS::ScriptComponent& component)
	{
		manager->InitEntity(ScriptType::CSharpScript, (uint32_t)entity, component.moduleName);
	}
	void ScriptCommand::ClearEntity(ECS::Entity entity, ECS::ScriptComponent& component)
	{
		manager->ClearEntity(ScriptType::CSharpScript, (uint32_t)entity, component.moduleName);
	}
	void ScriptCommand::InitEntity(ECS::Entity entity, ECS::VisualScriptComponent& component)
	{
		manager->InitEntity(ScriptType::CSharpScript, (uint32_t)entity, component.filename);
	}
	void ScriptCommand::ClearEntity(ECS::Entity entity, ECS::VisualScriptComponent& component)
	{
		manager->ClearEntity(ScriptType::CSharpScript, (uint32_t)entity, component.filename);
	}
	void ScriptCommand::SendDebugMessage(Severity severity, std::string message)
	{
		debugCallback(severity, message);
	}
}
