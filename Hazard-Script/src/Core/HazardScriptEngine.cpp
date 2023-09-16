
#include "HazardScriptEngine.h"

#include "ScriptCache.h"
#include <thread>

#include "Coral/StringHelper.hpp"

namespace HazardScript
{
	HazardScriptEngine* HazardScriptEngine::Create(HazardScriptCreateInfo* info)
	{
		return hnew HazardScriptEngine(info);
	}

	void HazardScriptEngine::SendDebugMessage(ScriptMessage message)
	{
		if (!s_Instance->m_DebugCallback) return;
		s_Instance->m_DebugCallback(message);
	}

	HazardScriptEngine::HazardScriptEngine(HazardScriptCreateInfo* info)
	{
		s_Instance = this;

		m_CoralData.CoreAssembly = Ref<ScriptAssembly>::Create();
		m_CoralData.AppAssembly = Ref<ScriptAssembly>::Create();

		m_CoralData.CoreAssembly->SetSourcePath(info->CoreAssemblyPath);
		m_CoralData.AppAssembly->SetSourcePath(info->AppAssemblyPath);

		m_CoralData.CoralDirectory = info->CoralDirectory;
		m_CoralData.BindingCallback = info->BindingCallback;
		m_CoralData.LoadAssembliesOnInit = info->LoadAssebmlies;

		HZR_ASSERT(info->DebugCallback, "Debug callback is required");
		HZR_ASSERT(info->BindingCallback, "Binding callback is required");

		m_DebugCallback = info->DebugCallback;
		SendDebugMessage({ Severity::Info, "Debug enabled" });

		InitializeCoralHost();
	}
	void HazardScriptEngine::Reload()
	{
		HZR_PROFILE_FUNCTION();
		
		m_CoralData.CoreAssembly->Unload(m_HostInstance);
		m_CoralData.AppAssembly->Unload(m_HostInstance);

		LoadCoreAssebly();
		LoadRuntimeAssembly();
		ReloadAppScripts();
	}
	void HazardScriptEngine::RegisterInternalCall(const std::string& signature, void* function)
	{
		//Mono::Register(signature, function);
	}
	void HazardScriptEngine::RunGarbageCollector() 
	{
		HZR_PROFILE_FUNCTION();
		/*
		mono_gc_collect(mono_gc_max_generation());
		using namespace std::chrono_literals;

		while (mono_gc_pending_finalizers()) 
			std::this_thread::sleep_for(500ns);
			*/
	}
	std::vector<Ref<ScriptAssembly>> HazardScriptEngine::GetAssemblies()
	{
		std::vector<Ref<ScriptAssembly>> assemblies;
		assemblies.push_back(s_Instance->m_CoralData.CoreAssembly);
		assemblies.push_back(s_Instance->m_CoralData.AppAssembly);
		return assemblies;
	}
	void HazardScriptEngine::InitializeCoralHost()
	{
		Coral::HostSettings settings = {};
		settings.CoralDirectory = m_CoralData.CoralDirectory.lexically_normal().string();
		settings.ErrorCallback = [](const CharType* message) {
			std::cout << Coral::StringHelper::ConvertWideToUtf8(message) << std::endl;
		};
		
		HZR_ASSERT(m_HostInstance.Initialize(settings), "Failed to initialize Coral");
		m_HostInstance.SetExceptionCallback([](const CharType* message) {
			std::cout << Coral::StringHelper::ConvertWideToUtf8(message) << std::endl;
		});

		//ScriptCache::Init();

		if (m_CoralData.LoadAssembliesOnInit)
			Reload();
	}
	void HazardScriptEngine::LoadCoreAssebly()
	{
		if (!m_CoralData.CoreAssembly->LoadAssembly(m_HostInstance))
		{
			SendDebugMessage({ Severity::Critical, "Core assembly loading failed" });
			return;
		}
	}
	void HazardScriptEngine::LoadRuntimeAssembly()
	{
		if (!m_CoralData.AppAssembly->LoadAssembly(m_HostInstance)) {
			SendDebugMessage({ Severity::Critical, "App assembly loading failed" });
			return;
		}

		m_CoralData.BindingCallback();
	}
	void HazardScriptEngine::ReloadAppScripts()
	{
		HZR_PROFILE_FUNCTION();
		for (auto& [name, script] : m_CoralData.AppAssembly->GetScripts())
		{
			for (auto& [handle, object] : script->GetAllInstances()) 
			{
				script->RegisterInstance(handle, object);
			}
		}
	}
}
