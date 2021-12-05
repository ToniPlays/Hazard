#pragma once

#include "hzrpch.h"
#include "ScriptCommand.h"
#include "Hazard/Entity/Component.h"
#include "Hazard/Entity/WorldHandler.h"

namespace Hazard::Scripting {

	void(*ScriptCommand::debugCallback)(Severity, const std::string&);
	ScriptEngineManager* ScriptCommand::s_Manager;

	void ScriptCommand::Init(ScriptEngineManager& manager)
	{
		s_Manager = &manager;
	}
	void ScriptCommand::OnBeginRuntime()
	{
		using namespace ECS;
		Ref<World> current = Application::GetModule<WorldHandler>().GetCurrentWorld();
		auto view = current->GetWorldRegistry().view<ScriptComponent>();

		for (auto entity : view) {

			Entity e = current->GetEntity(entity);
			auto& c = e.GetComponent<ScriptComponent>();

			if (ModuleExists(ScriptType::CSharpScript, c.m_ModuleName.c_str())) {
				s_Manager->Instantiate(ScriptType::CSharpScript, (uint32_t)entity, c.m_ModuleName);
			}
		}
	}
	void ScriptCommand::OnEndRuntime()
	{
		s_Manager->OnRuntimeEnd();
	}
	void ScriptCommand::InitAllEntities()
	{	
		Ref<ECS::World> world = ECS::WorldCommand::GetCurrentWorld();

		for (auto& [id, component] : world->FindEntitiesWith<ECS::ScriptComponent>()) 
		{
			if (!s_Manager->ModuleExists(ScriptType::CSharpScript, component.m_ModuleName.c_str()))
				continue;
			
			s_Manager->InitEntity(ScriptType::CSharpScript, (uint32_t)id, component.m_ModuleName);
		}
	}
	void ScriptCommand::InitEntity(UUID handle, ECS::ScriptComponent& component)
	{
		s_Manager->InitEntity(ScriptType::CSharpScript, handle, component.m_ModuleName);
	}
	void ScriptCommand::ClearEntity(UUID handle, ECS::ScriptComponent& component)
	{
		s_Manager->ClearEntity(ScriptType::CSharpScript, handle, component.m_ModuleName);
	}
	void ScriptCommand::InitEntity(UUID handle, ECS::VisualScriptComponent& component)
	{
		s_Manager->InitEntity(ScriptType::VisualScript, handle, component.m_Filename);
	}
	void ScriptCommand::ClearEntity(UUID handle, ECS::VisualScriptComponent& component)
	{
		s_Manager->ClearEntity(ScriptType::VisualScript, handle, component.m_Filename);
	}
	void ScriptCommand::SendDebugMessage(Severity severity, const std::string& message)
	{
		debugCallback(severity, message);
	}
	ScriptEngine& ScriptCommand::GetEngine(ScriptType type)
	{
		return s_Manager->GetEngine(type);
	}
}
