#pragma once

#include "hzrpch.h"
#include "ScriptEngineManager.h"
#include "ScriptCommand.h"

#include "Scripting/ScriptEngines.h"

namespace Hazard::Scripting {

	ScriptEngineManager::ScriptEngineManager() : Module("ScriptManager")
	{
		HZR_CORE_INFO("ScriptEngineManager()");
	}
	void ScriptEngineManager::Init()
	{
		scriptEngines.insert({ ScriptType::CSharpScript, new CSharp::CSharpEngine() });
		scriptEngines.insert({ ScriptType::VisualScript, new Visual::HVSEngine() });

		ScriptCommand::Init();
	}
	void ScriptEngineManager::Close()
	{

	}
	void ScriptEngineManager::Update() 
	{
		for (auto[type, engine] : scriptEngines) {
			engine->UpdateEntities();
		}
	}
	void ScriptEngineManager::Instantiate(ScriptType type, uint32_t entityID, std::string moduleName)
	{
		scriptEngines[type]->Instantiate(entityID, moduleName);
	}
	void ScriptEngineManager::InitEntity(ScriptType type, uint32_t entityID, std::string moduleName)
	{
		scriptEngines[type]->InitializeEntity(entityID, moduleName);
	}
	void ScriptEngineManager::ClearEntity(ScriptType type, uint32_t entityID, std::string moduleName)
	{
		scriptEngines[type]->ClearEntity(entityID, moduleName);
	}
	std::unordered_map<std::string, PublicField*> ScriptEngineManager::GetPublicFields(ScriptType type, uint32_t entity, const std::string& moduleName) {
		return scriptEngines[type]->GetPublicFields(entity, moduleName);
	}
	void ScriptEngineManager::ReloadAll()
	{
		for (auto [type, engine] : scriptEngines) {
			engine->Reload();
		}
		ScriptCommand::InitAllEntities();
	}
	bool ScriptEngineManager::ModuleExists(ScriptType type, const char* moduleName) {
		return scriptEngines[type]->ModuleExists(moduleName);
	}
}
