#pragma once

#include "hzrpch.h"
#include "ScriptCommand.h"
#include "Hazard/Entity/Component.h"
#include "Hazard/Entity/WorldHandler.h"

namespace Hazard::Scripting {

	void(*ScriptCommand::debugCallback)(Severity, const std::string&);
	ScriptEngineManager* ScriptCommand::s_manager;

	void ScriptCommand::Init()
	{
		s_manager = &Application::GetModule<ScriptEngineManager>();
	}
	void ScriptCommand::OnBeginRuntime()
	{
		using namespace ECS;
		World& current = Application::GetModule<WorldHandler>().GetCurrentWorld();
		auto view = current.GetWorldRegistry().view<ScriptComponent>();

		for (auto entity : view) {

			Entity e = current.GetEntity(entity);
			auto& c = e.GetComponent<ScriptComponent>();

			if (ModuleExists(ScriptType::CSharpScript, c.m_ModuleName.c_str())) {
				s_manager->Instantiate(ScriptType::CSharpScript, e, c.m_ModuleName);
			}
		}
	}
	void ScriptCommand::OnEndRuntime()
	{
		s_manager->OnRuntimeEnd();
	}
	void ScriptCommand::InitAllEntities()
	{	
		auto& scene = ECS::WorldCommand::GetCurrentWorld();
		for (auto [id, component] : scene.FindEntitiesWith<ECS::ScriptComponent>()) {
			if (s_manager->ModuleExists(ScriptType::CSharpScript, component.m_ModuleName.c_str()))
				s_manager->InitEntity(ScriptType::CSharpScript, id, component.m_ModuleName);
		}
	}
	void ScriptCommand::InitEntity(const ECS::Entity& entity, ECS::ScriptComponent& component)
	{
		s_manager->InitEntity(ScriptType::CSharpScript, (uint32_t)entity, component.m_ModuleName);
	}
	void ScriptCommand::ClearEntity(const ECS::Entity& entity, ECS::ScriptComponent& component)
	{
		s_manager->ClearEntity(ScriptType::CSharpScript, (uint32_t)entity, component.m_ModuleName);
	}

	void ScriptCommand::InitEntity(const ECS::Entity& entity, ECS::VisualScriptComponent& component)
	{
		s_manager->InitEntity(ScriptType::VisualScript, (uint32_t)entity, component.m_Filename);
	}
	void ScriptCommand::ClearEntity(const ECS::Entity& entity, ECS::VisualScriptComponent& component)
	{
		s_manager->ClearEntity(ScriptType::VisualScript, (uint32_t)entity, component.m_Filename);
	}
	void ScriptCommand::SendDebugMessage(Severity severity, const std::string& message)
	{
		debugCallback(severity, message);
	}
}
