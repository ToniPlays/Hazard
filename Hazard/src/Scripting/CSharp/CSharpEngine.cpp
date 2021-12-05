#pragma once

#include <hzrpch.h>
#include "CSharpEngine.h"
#include "Mono/Mono.h"
#include "CSharpField.h"
#include "ScriptUtils.h"
#include "CSharpScript.h"
#include "Hazard/Scripting/ScriptCommand.h"

extern "C"
{
	typedef struct _MonoObject MonoObject;
}

namespace Hazard::Scripting::CSharp {

	struct AssemblyData 
	{
		std::string assemblyPath;
	};

	AssemblyData data;

	CSharpEngine::CSharpEngine(ScriptEngineCreateInfo* info)
	{
		Mono::InitAssembly(info);
		Mono::CreateDomain("Hazard");
		Mono::LoadRuntimeAssembly(info->AppAssemblyPath);
	}
	void CSharpEngine::OnBeginRuntime()
	{

	}
	void CSharpEngine::OnEndRuntime()
	{

	}
	bool CSharpEngine::ModuleExists(const std::string& name)
	{
		return Mono::GetMonoClass(name.c_str());
	}
	void CSharpEngine::UpdateEntities()
	{
		float delta = Time::s_DeltaTime;
		void* param[] = { &delta };
		for (auto& [handle, instance] : m_Registry.GetRegisteredInstances()) {
			for (auto& script : instance.Scripts)
				Mono::TryCallMethod(Mono::ObjectFromHandle(script->GetHandle()), ((CSharpScript*)script)->OnUpdate, param);
		}
	}
	void CSharpEngine::OnWorldLoaded()
	{

	}
	void CSharpEngine::OnWorldUnloaded()
	{

	}
	PublicField* CSharpEngine::GetPublicField(uint32_t handle, uint32_t index)
	{
		return (PublicField*)m_Registry.GetInstanceData(handle).Scripts[index];
	}
	void CSharpEngine::InitializeEntity(uint32_t handle, const std::string& moduleName)
	{
		if (!ModuleExists(moduleName)) return;

		HZR_CORE_ERROR("Initializing {1} for handle {0}", handle, moduleName);

		if (!m_Registry.HasScript(moduleName)) {
			ScriptMetaData script;
			script.ModuleName = moduleName;
			ScriptUtils::GetNames(moduleName, script.Namespace, script.ClassName);
			m_Registry.Add(script);
		}

		CSharpScript* script = new CSharpScript(moduleName);
		script->InitClassMethods();
		script->UpdatePublicFields();
		m_Registry.RegisterEntityScript(handle, script);

	}
	void CSharpEngine::Instantiate(uint32_t handle, const std::string& moduleName)
	{
		InstanceData& data = m_Registry.GetInstanceData(handle);
		void* param[] = { &handle };

		for (auto& script : data.Scripts)
		{
			Mono::CallMethod(Mono::ObjectFromHandle(script->GetHandle()), ((CSharpScript*)script)->Constructor, param);
		}

		for (auto& script : data.Scripts) {
			((CSharpScript*)script)->SetRuntimeValues();
		}

		for (auto& script : data.Scripts)
		{
			Mono::TryCallMethod(Mono::ObjectFromHandle(script->GetHandle()), ((CSharpScript*)script)->OnCreated, param);
		}
		for (auto& script : data.Scripts)
		{
			Mono::TryCallMethod(Mono::ObjectFromHandle(script->GetHandle()), ((CSharpScript*)script)->OnStart, param);
		}
	}
	void CSharpEngine::ClearEntity(uint32_t handle, const std::string& moduleName)
	{
		m_Registry.Remove(handle);
	}
	void CSharpEngine::OnFixedUpdate(uint32_t handle)
	{

	}
	void CSharpEngine::OnEnable(uint32_t handle)
	{

	}
	void CSharpEngine::OnDisable(uint32_t handle)
	{

	}
	void CSharpEngine::OnDestroy(uint32_t handle)
	{

	}
	bool CSharpEngine::EntityInstanceExits(uint32_t handle)
	{
		return false;
	}
	void CSharpEngine::OnApplicationClose()
	{
		Mono::Shutdown();
	}
	void CSharpEngine::Reload()
	{
		Mono::LoadRuntimeAssembly("c:/dev/HazardProject/bin/Debug/netstandard2.0/HazardProject.dll");
	}
}