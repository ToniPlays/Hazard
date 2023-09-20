
#include <hzrpch.h>
#include "ScriptEngine.h"
#include "InternalCalls.h"
#include "ScriptAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "Attributes/AttributeConstructor.h"
#include "MonoUtilities.h"

#define TYPEDEF(x, y) if (strcmp(##name, x) == 0) return y;

using namespace HazardScript;

namespace Hazard
{
	ScriptEngine::ScriptEngine(ScriptEngineCreateInfo* info) : Module("ScriptEngine")
	{
		m_Info = *info;
	}
	void ScriptEngine::PreInit()
	{
		AssetManager::RegisterLoader<ScriptAssetLoader>(AssetType::Script, this);

		HZR_PROFILE_FUNCTION();

		HazardScriptCreateInfo createInfo = {};
		createInfo.CoreAssemblyPath = m_Info.CoreAssemblyPath;
		createInfo.AppAssemblyPath = m_Info.AppAssemblyPath;
		createInfo.CoralDirectory = m_Info.CoralDirectory;

		createInfo.DebugCallback = [&](ScriptMessage message) {
			if (!m_MessageCallback)
			{
				m_QueuedMessages.push_back(message);
				return;
			}

			for (auto m : m_QueuedMessages)
			{
				m_MessageCallback(m);
			}
			m_QueuedMessages.clear();
			m_MessageCallback(message);
		};
		createInfo.BindingCallback = [&](Ref<ScriptAssembly> assembly) {
			if (m_ScriptGlue.contains(assembly.Raw()))
			{
				for (auto& glue : m_ScriptGlue[assembly.Raw()])
				{
					glue->OnAssemblyLoaded(assembly);
					glue->Register(assembly);
				}
				assembly->UploadInternalCalls();
			}
		};
		AttributeConstructor::Init();

		m_Engine = HazardScriptEngine::Create(&createInfo);
		Ref<ScriptAssembly> coreAssembly = m_Engine->GetLoadedAssembly("HazardScripting");

		RegisterScriptGlueFor<InternalCalls>(coreAssembly);

		m_Engine->Reload();
	}
	void ScriptEngine::Update()
	{
		HZR_PROFILE_FUNCTION();
	}

	bool ScriptEngine::FindModule(const std::string& moduleName)
	{
		for (auto& assembly : m_Engine->GetAssemblies())
		{
			if (assembly->HasScript(moduleName))
				return true;
		}
		return false;
	}

	ScriptMetadata ScriptEngine::GetScript(const std::string& moduleName)
	{
		for (auto& assembly : m_Engine->GetAssemblies())
		{
			if (assembly->HasScript(moduleName))
				return assembly->GetScript(moduleName);
		}
		return ScriptMetadata();
	}

	void ScriptEngine::SendDebugMessage(const ScriptMessage& message)
	{
		HZR_CORE_INFO(message.Message);
		m_Engine->SendDebugMessage(message);
	}
	void ScriptEngine::ReloadAssemblies()
	{
		m_Engine->Reload();
	}
	void ScriptEngine::SetDebugCallback(ScriptMessageCallback callback)
	{
		m_MessageCallback = callback;

		for (auto& m : m_QueuedMessages)
			m_MessageCallback(m);

		m_QueuedMessages.clear();
	}
	void ScriptEngine::InitializeComponent(const Entity& entity, ScriptComponent& component)
	{
		HZR_PROFILE_FUNCTION();
		if (component.ModuleName == "") return;
		if (!FindModule(component.ModuleName)) return;

		ScriptMetadata script = GetScript(component.ModuleName);
		component.m_Handle = script.CreateObject<uint64_t>((uint64_t)entity.GetUID());
	}
	const std::vector<Ref<ScriptAssembly>>& ScriptEngine::GetAssemblies()
	{
		return m_Engine->GetAssemblies();
	}
}
