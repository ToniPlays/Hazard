
#include "HazardScriptEngine.h"

#include "Buffer.h"
#include "Mono/Core/Mono.h"
#include "ScriptCache.h"
#include <thread>

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

		m_MonoData.CoreAssembly = Ref<ScriptAssembly>::Create();
		m_MonoData.AppAssembly = Ref<ScriptAssembly>::Create();

		m_MonoData.CoreAssembly->SetSourcePath(info->CoreAssemblyPath);
		m_MonoData.AppAssembly->SetSourcePath(info->AppAssemblyPath);

		m_MonoData.MonoAssemblyDir = info->AssemblyPath;
		m_MonoData.MonoConfigDir = info->ConfigPath;
		m_MonoData.BindingCallback = info->BindingCallback;
		m_MonoData.LoadAssembliesOnInit = info->LoadAssebmlies;

		HZR_ASSERT(info->DebugCallback, "Debug callback is required");
		HZR_ASSERT(info->BindingCallback, "Binding callback is required");

		m_DebugCallback = info->DebugCallback;
		SendDebugMessage({ Severity::Info, "Debug enabled" });

		InitializeMono();
	}
	void HazardScriptEngine::Reload()
	{
		HZR_PROFILE_FUNCTION();
		LoadCoreAssebly();
		LoadRuntimeAssembly();
		ReloadAppScripts();
	}
	void HazardScriptEngine::RegisterInternalCall(const std::string& signature, void* function)
	{
		Mono::Register(signature, function);
	}
	void HazardScriptEngine::RunGarbageCollector() 
	{
		HZR_PROFILE_FUNCTION();
		mono_gc_collect(mono_gc_max_generation());
		using namespace std::chrono_literals;

		while (mono_gc_pending_finalizers()) 
			std::this_thread::sleep_for(500ns);
	}
	std::vector<Ref<ScriptAssembly>> HazardScriptEngine::GetAssemblies()
	{
		std::vector<Ref<ScriptAssembly>> assemblies;
		assemblies.push_back(s_Instance->m_MonoData.CoreAssembly);
		assemblies.push_back(s_Instance->m_MonoData.AppAssembly);
		return assemblies;
	}
	void HazardScriptEngine::CheckError(MonoObject* exception, MonoObject* result, MonoMethod* method)
	{
		MonoClass* exceptionClass = mono_object_get_class(exception);
		MonoType* type = mono_class_get_type(exceptionClass);
		std::string typeName = mono_type_get_name(type);

		if (strcmp(typeName.c_str(), "System.MissingMethodException") == 0)
		{
			std::string methodName = mono_method_get_reflection_name(method);
			std::string stackTrace = "Encountered missing method when executing " + methodName;
			HazardScriptEngine::SendDebugMessage({ Severity::Error, "Missing method exception on " + methodName, stackTrace });
			return;
		}

		MonoObject* traceObject = mono_object_new(Mono::GetDomain(), ScriptCache::GetManagedClassByName("System.Diagnostics.StackTrace")->Class);
		MonoObject* traceExcept = nullptr;
		MonoString* stackTraceString = mono_object_to_string(traceObject, &traceExcept);

		std::string message = Mono::GetStringProperty("Message", exceptionClass, exception);
		std::string stackTrace = Mono::GetStringProperty("StackTrace", exceptionClass, exception);

		HazardScriptEngine::SendDebugMessage({ Severity::Error, typeName + ": " + message, message + "\n\n" + stackTrace });
	}
	void HazardScriptEngine::InitializeMono()
	{
		Mono::SetDirs(m_MonoData.MonoAssemblyDir, m_MonoData.MonoConfigDir);

		Mono::Init("HazardScriptCore");
		ScriptCache::Init();

		if (m_MonoData.LoadAssembliesOnInit) 
		{
			Reload();
		}
	}
	void HazardScriptEngine::LoadCoreAssebly()
	{
		MonoDomain* domain = nullptr;
		bool cleanup = false;

		if (!Mono::Init("HazardScriptCore")) {
			SendDebugMessage({ Severity::Critical, "Failed to initialize Mono" });
			return;
		}

		if (!m_MonoData.CoreAssembly->LoadFromSource(true))
		{
			SendDebugMessage({ Severity::Critical, "Core assembly loading failed" });
			return;
		}
	}
	void HazardScriptEngine::LoadRuntimeAssembly()
	{
		if (!m_MonoData.AppAssembly->LoadFromSource(true, true)) {
			SendDebugMessage({ Severity::Critical, "App assembly loading failed" });
			return;
		}
		m_MonoData.BindingCallback();
	}
	void HazardScriptEngine::ReloadAppScripts()
	{
		HZR_PROFILE_FUNCTION();
		for (auto& [name, script] : m_MonoData.AppAssembly->GetScripts()) 
		{
			for (auto& [handle, object] : script->GetAllInstances()) 
			{
				script->RegisterInstance(handle, object);
			}
		}
	}
}