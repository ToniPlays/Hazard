#pragma once

#include <hzrpch.h>
#include "CSharpEngine.h"
#include "Mono/Mono.h"
#include "CSharpField.h"
#include "ScriptUtils.h"

namespace Hazard::Scripting::CSharp {

	CSharpEngine::CSharpEngine()
	{
		stats.name = "CSharp";
		stats.assemblyPath = "c:/dev/HazardProject/bin/Debug/netstandard2.0/HazardProject.dll";

		Mono::InitAssembly(stats.assemblyPath);
		Mono::CreateDomain("Hazard");
		Mono::LoadRuntimeAssembly(stats.assemblyPath);
	}
	void CSharpEngine::OnBeginRuntime()
	{
		HZR_INFO("CSharp begin for {0} entities", entities.size());

		for (auto [entityID, data] : entities) {
			uint32_t id = entityID;
			void* param[] = { &id };
			Mono::CallMethod(data.data.scriptClass, data.Constructor, param);

			if (data.OnCreate != nullptr)
				Mono::CallMethod(data.data.scriptClass, data.OnCreate);

			if (data.OnStart != nullptr)
				Mono::CallMethod(data.data.scriptClass, data.OnStart);
		}
	}
	void CSharpEngine::OnEndRuntime()
	{
		HZR_INFO("CSharp end");
	}
	bool CSharpEngine::ModuleExists(const char* name)
	{
		return Mono::ModuleExists(name);
	}
	void CSharpEngine::UpdateEntities()
	{
		for (auto [entityID, data] : entities) {
			if (data.OnUpdate == nullptr) continue;
			void* param[] = { &Time::deltaTime };
			Mono::CallMethod(data.data.scriptClass, data.OnUpdate, param);
		}
	}
	void CSharpEngine::OnSceneLoaded()
	{
		HZR_INFO("CSharp scene loaded");
	}
	void CSharpEngine::OnSceneUnloaded()
	{
		HZR_INFO("CSharp scene unloaded");
	}
	void CSharpEngine::InitializeEntity(uint32_t entity, std::string moduleName)
	{
		ScriptData data = ScriptData(moduleName, Mono::GetMonoClass(moduleName.c_str()));
		auto fields = Mono::GetClassFields(data.scriptClass);
		data.fields = ScriptUtils::MonoFieldsToFields(fields);

		scripts[entity] = data;
		MonoEntity instance(data);
		instance.handle = Mono::InstantiateHandle(data.scriptClass);
		entities[entity] = instance;
	}
	void CSharpEngine::ClearEntity(uint32_t entity, std::string moduleName)
	{
		auto it = scripts.find(entity);
		if (it != scripts.end()) {
			scripts.erase(it);
		}
		HZR_CORE_INFO("C# script count {0}", scripts.size());
	}
	void CSharpEngine::OnCreate(uint32_t entity)
	{
		HZR_INFO("CSharp entity created");
	}
	void CSharpEngine::OnStart(uint32_t entity)
	{
		HZR_INFO("CSharp entity start");
	}
	void CSharpEngine::OnUpdate(uint32_t entity)
	{
		HZR_INFO("CSharp entity update");
	}
	void CSharpEngine::OnLateUpdate(uint32_t entity)
	{
		HZR_INFO("CSharp late update");
	}
	void CSharpEngine::OnFixedUpdate(uint32_t entity)
	{
		HZR_INFO("CSharp fixed update");
	}
	void CSharpEngine::OnEnable(uint32_t entity)
	{
		HZR_INFO("CSharp entity disabled");
	}
	void CSharpEngine::OnDisable(uint32_t entity)
	{
		HZR_INFO("CSharp entity enabled");
	}
	void CSharpEngine::OnDestroy(uint32_t entity)
	{
		HZR_INFO("CSharp entity destroyed");
	}
	void CSharpEngine::OnCollision(uint32_t entity)
	{
		HZR_INFO("CSharp entity collision");
	}
	void CSharpEngine::OnApplicationClose()
	{
		HZR_INFO("CSharp Application close");
	}
	void CSharpEngine::Reload()
	{
		Mono::LoadRuntimeAssembly(stats.assemblyPath);
	}
	ScriptData& CSharpEngine::GetData(uint32_t entity, const std::string& moduleName)
	{
		return scripts[entity];
	}
}