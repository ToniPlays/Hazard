#pragma once

#include <hzrpch.h>
#include "HVSEngine.h"

namespace Hazard::Scripting::Visual {
	HVSEngine::HVSEngine()
	{
		stats.name = "Hazard Visual Scripting";
		stats.assemblyPath = "No assemblies";
	}
	void HVSEngine::OnBeginRuntime()
	{
		HZR_INFO("HVSEngine begin");
	}
	void HVSEngine::OnEndRuntime()
	{
		HZR_INFO("HVSEngine end");
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
		HZR_INFO("HVSEngine scene loaded");
	}
	void HVSEngine::OnSceneUnloaded()
	{
		HZR_INFO("HVSEngine scene unloaded");
	}
	void HVSEngine::InitializeEntity(uint32_t entity, std::string moduleName)
	{
		HZR_INFO("HVSEngine entity initialized");
	}
	void HVSEngine::ClearEntity(uint32_t entity, std::string moduleName)
	{

	}
	void HVSEngine::OnCreate(uint32_t entity)
	{
		HZR_INFO("HVSEngine entity created");
	}
	void HVSEngine::OnStart(uint32_t entity)
	{
		HZR_INFO("HVSEngine entity start");
	}
	void HVSEngine::OnUpdate(uint32_t entity)
	{
		HZR_INFO("HVSEngine entity update");
	}
	void HVSEngine::OnLateUpdate(uint32_t entity)
	{
		HZR_INFO("HVSEngine late update");
	}
	void HVSEngine::OnFixedUpdate(uint32_t entity)
	{
		HZR_INFO("HVSEngine fixed update");
	}
	void HVSEngine::OnEnable(uint32_t entity)
	{
		HZR_INFO("HVSEngine entity disabled");
	}
	void HVSEngine::OnDisable(uint32_t entity)
	{
		HZR_INFO("HVSEngine entity enabled");
	}
	void HVSEngine::OnDestroy(uint32_t entity)
	{
		HZR_INFO("HVSEngine entity destroyed");
	}
	void HVSEngine::OnCollision(uint32_t entity)
	{
		HZR_INFO("HVSEngine entity collision");
	}
	void HVSEngine::OnApplicationClose()
	{
		HZR_INFO("HVSEngine Application close");
	}
	void HVSEngine::Reload()
	{
		HZR_CORE_INFO("HVSEngine reload");
	}
	ScriptData& HVSEngine::GetData(uint32_t entity, const std::string& moduleName)
	{
		ScriptData data;
		return data;
	}
}