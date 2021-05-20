#pragma once

#include <hzrpch.h>
#include "HVSEngine.h"

namespace Hazard::Scripting::Visual {

	HVSEngine::HVSEngine(ScriptEngineCreateInfo* info)
	{
		HZR_PROFILE_FUNCTION();
	}
	void HVSEngine::OnBeginRuntime()
	{

	}
	void HVSEngine::OnEndRuntime()
	{

	}
	bool HVSEngine::ModuleExists(const char* name)
	{
		return false;
	}
	void HVSEngine::UpdateEntities()
	{
	}

	void HVSEngine::OnSceneLoaded()
	{

	}
	void HVSEngine::OnSceneUnloaded()
	{

	}
	std::unordered_map<std::string, PublicField*> HVSEngine::GetPublicFields(uint32_t entity, const std::string& moduleName)
	{
		return std::unordered_map<std::string, PublicField*>();
	}
	void HVSEngine::InitializeEntity(uint32_t entity, const std::string& moduleName)
	{

	}
	void HVSEngine::Instantiate(uint32_t entity, const std::string& moduleName)
	{

	}
	void HVSEngine::ClearEntity(uint32_t entity, const std::string& moduleName)
	{

	}
	void HVSEngine::OnCreate(uint32_t entity)
	{

	}
	void HVSEngine::OnStart(uint32_t entity)
	{

	}
	void HVSEngine::OnUpdate(uint32_t entity)
	{

	}
	void HVSEngine::OnLateUpdate(uint32_t entity)
	{

	}
	void HVSEngine::OnFixedUpdate(uint32_t entity)
	{

	}
	void HVSEngine::OnEnable(uint32_t entity)
	{

	}
	void HVSEngine::OnDisable(uint32_t entity)
	{

	}
	void HVSEngine::OnDestroy(uint32_t entity)
	{

	}
	void HVSEngine::OnCollision(uint32_t entity)
	{

	}
	void HVSEngine::OnApplicationClose()
	{
		HZR_PROFILE_FUNCTION();
	}
	void HVSEngine::Reload()
	{

	}
}