
#include <hzrpch.h>
#include "ScriptEngine.h"
#include "ScriptBindings.h"

using namespace HazardScript;

namespace Hazard 
{
	ScriptEngine::ScriptEngine(ScriptEngineCreateInfo* info) : Module("ScriptEngine")
	{
		HazardScriptCreateInfo createInfo = {};
		createInfo.CoreAssemblyPath = info->CoreAssemblyPath;
		createInfo.AppAssemblyPath = info->AppAssemblyPath;
		createInfo.AssemblyPath = info->AssemblyPath;
		createInfo.ConfigPath = info->ConfigPath;

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
			for (auto& cb : m_BindCallbacks) {
				cb(this);
			}
		};

		AddBindingCallback(Internal::RegisterInternalCalls);
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