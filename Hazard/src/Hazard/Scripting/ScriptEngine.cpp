
#include <hzrpch.h>
#include "ScriptEngine.h"
#include "ScriptBindings.h"
#include "ScriptAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"

using namespace HazardScript;

namespace Hazard 
{
	ScriptEngine::ScriptEngine(ScriptEngineCreateInfo* info) : Module("ScriptEngine")
	{
		m_Info = *info;
		RegisterScriptGlue<InternalCall>();

		AssetManager::RegisterLoader<ScriptAssetLoader>(AssetType::Script, this);

	}
	void ScriptEngine::Init()
	{
		HazardScriptCreateInfo createInfo = {};
		createInfo.CoreAssemblyPath = m_Info.CoreAssemblyPath;
		createInfo.AppAssemblyPath = m_Info.AppAssemblyPath;
		createInfo.AssemblyPath = m_Info.AssemblyPath;
		createInfo.ConfigPath = m_Info.ConfigPath;
		createInfo.LoadAssebmlies = false;

		createInfo.DebugCallback = [&](ScriptMessage message)
		{
			if (!m_MessageCallback) {
				m_QueuedMessages.push_back(message);
				return;
			}

			for (auto m : m_QueuedMessages) {
				m_MessageCallback(m);
			}
			m_QueuedMessages.clear();
			m_MessageCallback(message);
		};
		createInfo.BindingCallback = [&]() {
			HZR_CORE_WARN("Binding stuff");
			for (auto& cb : m_ScriptGlue) {
				for (auto* assembly : m_Engine->GetAssemblies())
					cb->OnAssemblyLoaded(assembly);
			}


			for (auto& cb : m_ScriptGlue) {
				cb->Register(this);
			}
		};

		m_Engine = HazardScriptEngine::Create(&createInfo);
	}
	bool ScriptEngine::HasModule(const std::string& moduleName)
	{
		return m_Engine->GetAppAssembly().HasScript(moduleName);
	}
	Script& ScriptEngine::GetScript(const std::string& moduleName) {
		return m_Engine->GetAppAssembly().GetScript(moduleName);
	}
	void ScriptEngine::SendDebugMessage(const ScriptMessage& message)
	{
		m_Engine->SendDebugMessage(message);
	}
	void ScriptEngine::ReloadAssemblies()
	{
		m_Engine->Reload();
	}
	void ScriptEngine::SetDebugCallback(ScriptMessageCallback callback)
	{
		m_MessageCallback = callback;
		for (auto& m : m_QueuedMessages) {
			m_MessageCallback(m);
		}
		m_QueuedMessages.clear();
	}
	void ScriptEngine::InitializeComponent(Entity& entity, ScriptComponent& component)
	{
		if (component.ModuleName == "") return;
		if (!m_Engine->GetAppAssembly().HasScript(component.ModuleName)) return;
		Script& script = m_Engine->GetAppAssembly().GetScript(component.ModuleName);
		component.m_Handle = script.CreateObject();

		uint32_t entityID = entity;
		component.m_Handle->GetScript().ValidateOrLoadMethod("Hazard.Entity:.ctor(ulong)");

		void* params[] = { &entityID };
		component.m_Handle->Invoke("Hazard.Entity:.ctor(ulong)", params);
		component.m_Handle->TryInvoke("OnCreate()", params);
	}
}