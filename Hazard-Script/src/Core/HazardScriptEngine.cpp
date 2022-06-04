
#include "HazardScriptEngine.h"

#include "Buffer.h"
#include "Mono/Core/Mono.h"

namespace HazardScript 
{
	HazardScriptEngine* HazardScriptEngine::Create(HazardScriptCreateInfo* info)
	{
		return new HazardScriptEngine(info);
	}

	void HazardScriptEngine::SendDebugMessage(ScriptMessage message)
	{
		if (!s_Instance->m_DebugCallback) return;
		s_Instance->m_DebugCallback(message);
	}

	HazardScriptEngine::HazardScriptEngine(HazardScriptCreateInfo* info) 
	{
		s_Instance = this;

		m_MonoData.CoreAssembly.SetSourcePath(info->CoreAssemblyPath);
		m_MonoData.AppAssembly.SetSourcePath(info->AppAssemblyPath);
		m_MonoData.MonoAssemblyDir = info->AssemblyPath;
		m_MonoData.MonoConfigDir = info->ConfigPath;
		m_MonoData.BindingCallback = info->BindingCallback;

		m_DebugCallback = info->DebugCallback;
		SendDebugMessage({ Severity::Info, "Debug enabled" });

		InitializeMono();
	}
	void HazardScriptEngine::RegisterInternalCall(const std::string& signature, void* function) 
	{
		Mono::Register(signature, function);
	}
	void HazardScriptEngine::InitializeMono()
	{
		Mono::SetDirs(m_MonoData.MonoAssemblyDir, m_MonoData.MonoConfigDir);

		Mono::Init("HazardScriptCore");
		
		LoadCoreAssebly();
		LoadRuntimeAssembly();
	}
	void HazardScriptEngine::LoadCoreAssebly()
	{
		MonoDomain* domain = nullptr;
		bool cleanup = false;

		Mono::Init("HazardScriptCore");

		m_MonoData.CoreAssembly.LoadFromSource();
		m_MonoData.AppAssembly.LoadFromSource(true);
		SendDebugMessage({ Severity::Info, "Assemblies loaded" });
	}
	void HazardScriptEngine::LoadRuntimeAssembly()
	{
		m_MonoData.AppAssembly.LoadFromSource();
		m_MonoData.BindingCallback();
	}
}