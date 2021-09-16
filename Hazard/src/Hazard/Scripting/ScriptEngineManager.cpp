#pragma once

#include "hzrpch.h"
#include "ScriptEngineManager.h"
#include "ScriptCommand.h"
#include "Scripting/ScriptEngines.h"
#include "Hazard/Physics/PhysicsCommand.h"

namespace Hazard::Scripting {

	ScriptEngineManager::ScriptEngineManager() : Module("ScriptManager")
	{

	}
	void ScriptEngineManager::InitEngines(ScriptEngineCreateInfo* info)
	{
		HZR_PROFILE_FUNCTION();
		m_ScriptEngines.insert({ ScriptType::CSharpScript, new CSharp::CSharpEngine(info) });
		m_ScriptEngines.insert({ ScriptType::VisualScript, new Visual::HVSEngine(info) });

		ScriptCommand::Init();
		SetActive(info->enable);
	}
	void ScriptEngineManager::Close()
	{
		HZR_PROFILE_FUNCTION();
		for (auto [type, engine] : m_ScriptEngines) {
			engine->OnApplicationClose();
		}
	}
	void ScriptEngineManager::Update() 
	{
		Physics::PhysicsCommand::UpdateAll(&ECS::WorldCommand::GetCurrentWorld());

		for (auto[type, engine] : m_ScriptEngines) {
			engine->UpdateEntities();
		}
	}
	void ScriptEngineManager::Instantiate(ScriptType type, uint32_t entityID, const std::string& moduleName)
	{
		m_ScriptEngines[type]->Instantiate(entityID, moduleName);
	}
	void ScriptEngineManager::OnRuntimeEnd()
	{
		for (auto [type, engine] : m_ScriptEngines) {
			engine->OnEndRuntime();
		}
	}
	void ScriptEngineManager::InitEntity(ScriptType type, uint32_t entityID, const std::string& moduleName)
	{
		m_ScriptEngines[type]->InitializeEntity(entityID, moduleName);
	}
	void ScriptEngineManager::ClearEntity(ScriptType type, uint32_t entityID, const std::string& moduleName)
	{
		m_ScriptEngines[type]->ClearEntity(entityID, moduleName);
	}
	std::unordered_map<std::string, PublicField*> ScriptEngineManager::GetPublicFields(ScriptType type, uint32_t entity, const std::string& moduleName) {
		return m_ScriptEngines[type]->GetPublicFields(entity, moduleName);
	}
	void ScriptEngineManager::ReloadAll()
	{
		HZR_CORE_INFO("Reloading assemblies");
		for (auto [type, engine] : m_ScriptEngines) {
			engine->Reload();
		}
		ScriptCommand::InitAllEntities();
	}
	bool ScriptEngineManager::ModuleExists(ScriptType type, const char* moduleName) {
		return m_ScriptEngines[type]->ModuleExists(moduleName);
	}
}
