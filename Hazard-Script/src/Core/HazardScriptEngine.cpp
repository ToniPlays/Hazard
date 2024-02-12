
#include "HazardScriptEngine.h"

#include <thread>
#include "Utility/StringUtil.h"

#include "Coral/StringHelper.hpp"
#include <Coral/GC.hpp>

namespace HazardScript
{
	HazardScriptEngine* HazardScriptEngine::Create(HazardScriptCreateInfo* info)
	{
		return hnew HazardScriptEngine(info);
	}

	void HazardScriptEngine::SendDebugMessage(ScriptMessage message)
	{
		s_Instance->m_DebugCallbacks.Invoke<ScriptMessage&>(message);
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

		m_DebugCallbacks.Add(info->DebugCallback);
		SendDebugMessage({ Severity::Info, "Debug enabled" });

		InitializeCoralHost();

		m_Assemblies.clear();
		m_Assemblies.push_back(m_CoralData.CoreAssembly);
		m_Assemblies.push_back(m_CoralData.AppAssembly);
	}
	void HazardScriptEngine::Reload()
	{
		return;
		HZR_PROFILE_FUNCTION();
		std::scoped_lock lock(m_ReloadMutex);
		Coral::GC::Collect();

		m_HostInstance.UnloadAssemblyLoadContext(m_LoadContext);
		m_LoadContext = m_HostInstance.CreateAssemblyLoadContext("Context");

		LoadAssembly(m_CoralData.CoreAssembly);
		LoadAssembly(m_CoralData.AppAssembly);

		m_Assemblies.clear();
		m_Assemblies.push_back(m_CoralData.CoreAssembly);
		m_Assemblies.push_back(m_CoralData.AppAssembly);
	}
	void HazardScriptEngine::RunGarbageCollector()
	{
		HZR_PROFILE_FUNCTION();
	}
	bool HazardScriptEngine::LoadAssembly(Ref<ScriptAssembly> assembly)
	{
		bool succeeded = false;

		succeeded = assembly->LoadAssembly(m_HostInstance, m_LoadContext);
		
		if (succeeded)
			m_CoralData.BindingCallback(assembly);

		return succeeded;
	}
	const std::vector<Ref<ScriptAssembly>>& HazardScriptEngine::GetAssemblies()
	{
		return m_Assemblies;
	}
	void HazardScriptEngine::InitializeCoralHost()
	{
		std::string coralDir = m_CoralData.CoralDirectory.lexically_normal().string();

		Coral::HostSettings settings = {
			.CoralDirectory = coralDir,
			.MessageCallback = [&](std::string_view message, Coral::MessageLevel level) {
				std::string msg = std::string(message);
				std::string exception = msg.substr(0, msg.find_first_of("\n"));
				std::string trace = exception.length() + 1 < msg.length() ? msg.substr(exception.length() + 1) : "";
				m_DebugCallbacks.Invoke<ScriptMessage>({ Severity::Error, exception, trace });
				std::cout << msg << std::endl;
			},
			.ExceptionCallback = ([&](std::string_view message) {
				std::string msg = std::string(message);
				std::string exception = msg.substr(0, msg.find_first_of("\n"));
				std::string trace = exception.length() + 1 < message.length() ? msg.substr(exception.length() + 1) : "";
				m_DebugCallbacks.Invoke<ScriptMessage>({ Severity::Error, exception, trace });
				std::cout << msg << std::endl;
			}),
		};

		//HZR_ASSERT(m_HostInstance.Initialize(settings), "Failed to initialize Coral");
	}
}
