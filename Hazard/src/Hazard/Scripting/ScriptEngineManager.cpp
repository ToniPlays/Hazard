#pragma once

#include "hzrpch.h"
#include "ScriptEngineManager.h"
#include "ScriptCommand.h"
#include "Scripting/ScriptEngines.h"
#include "Hazard/Physics/PhysicsCommand.h"

namespace Hazard::Scripting {

	ScriptEngineManager::ScriptEngineManager(ScriptEngineCreateInfo* info) : Module("ScriptManager") 
	{
		HZR_PROFILE_FUNCTION();
		m_ScriptEngines[ScriptType::CSharpScript] = new CSharp::CSharpEngine(info);

		ScriptCommand::Init(*this);
		SetActive(info->Enable);
		//TODO: Move to module
		Physics::PhysicsCommand::Init();
	}
	ScriptEngineManager::~ScriptEngineManager()
	{

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
		//TODO: Same here
		Physics::PhysicsCommand::UpdateAll(ECS::WorldCommand::GetCurrentWorld());

		for (auto[type, engine] : m_ScriptEngines) {
			engine->UpdateEntities();
		}
	}
	void ScriptEngineManager::Instantiate(ScriptType type, UUID handle, const std::string& moduleName)
	{
		m_ScriptEngines[type]->Instantiate(handle, moduleName);
	}
	void ScriptEngineManager::OnRuntimeEnd()
	{
		for (auto [type, engine] : m_ScriptEngines) {
			engine->OnEndRuntime();
		}
	}
	void ScriptEngineManager::InitEntity(ScriptType type, UUID handle, const std::string& moduleName)
	{
		m_ScriptEngines[type]->InitializeEntity(handle, moduleName);
	}
	void ScriptEngineManager::ClearEntity(ScriptType type, UUID handle, const std::string& moduleName)
	{
		m_ScriptEngines[type]->ClearEntity(handle, moduleName);
	}
	std::unordered_map<std::string, PublicField*> ScriptEngineManager::GetPublicFields(ScriptType type, UUID handle, const std::string& moduleName)
	{
		return m_ScriptEngines[type]->GetPublicFields(handle, moduleName);
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
