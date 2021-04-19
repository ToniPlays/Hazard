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
	void ScriptEngineManager::BeginRuntime()
	{
		for (auto [type, engine] : scriptEngines) {
			engine->OnBeginRuntime();
		}
	}
	void ScriptEngineManager::EndRuntime()
	{
		for (auto [type, engine] : scriptEngines) {
			engine->OnEndRuntime();
		}
	}
	void ScriptEngineManager::InitEntity(uint32_t entityID, std::string moduleName, ScriptType type)
	{
		scriptEngines[type]->InitializeEntity(entityID, moduleName);
	}
	void ScriptEngineManager::ClearEntity(uint32_t entityID, std::string moduleName, ScriptType type)
	{
		scriptEngines[type]->ClearEntity(entityID, moduleName);
	}
	void ScriptEngineManager::ReloadAll()
	{
		for (auto [type, engine] : scriptEngines) {
			engine->Reload();
		}
	}
	bool ScriptEngineManager::ModuleExists(ScriptType type, const char* moduleName) {
		return scriptEngines[type]->ModuleExists(moduleName);
	}
	ScriptData ScriptEngineManager::GetData(ScriptType type, uint32_t entity, std::string moduleName) {
		return scriptEngines[type]->GetData(entity, moduleName);
	}
}
