
#include "HazardScriptEngine.h"

#include "Buffer.h"
#include "Mono/Core/Mono.h"
#include "ScriptCache.h"

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
		m_MonoData.LoadAssembliesOnInit = info->LoadAssebmlies;

		HZR_ASSERT(info->DebugCallback, "Debug callback is required");
		HZR_ASSERT(info->BindingCallback, "Binding callback is required");

		m_DebugCallback = info->DebugCallback;
		SendDebugMessage({ Severity::Info, "Debug enabled" });

		InitializeMono();
	}
	void HazardScriptEngine::Reload()
	{
		LoadCoreAssebly();
		LoadRuntimeAssembly();
	}
	void HazardScriptEngine::RegisterInternalCall(const std::string& signature, void* function)
	{
		Mono::Register(signature, function);
	}
	void HazardScriptEngine::RunGarbageCollector() {
		//mono_gc_collect(mono_gc_max_generation());
	}
	std::vector<ScriptAssembly*> HazardScriptEngine::GetAssemblies()
	{
		std::vector<ScriptAssembly*> assemblies;
		assemblies.push_back(&s_Instance->m_MonoData.CoreAssembly);
		assemblies.push_back(&s_Instance->m_MonoData.AppAssembly);
		return assemblies;
	}
	void HazardScriptEngine::CheckError(MonoObject* exception, MonoObject* result, MonoMethod* method)
	{
		MonoClass* exceptionClass = mono_object_get_class(exception);
		MonoType* type = mono_class_get_type(exceptionClass);
		const char* typeName = mono_type_get_name(type);

		if (strcmp(typeName, "System.MissingMethodException") == 0) {
			std::string methodName = mono_method_get_reflection_name(method);
			std::string stackTrace = "Encountered missing method when executing " + methodName;
			HazardScriptEngine::SendDebugMessage({ Severity::Error, "Missing method exception on " + methodName, stackTrace });
			return;
		}

		std::string stacktrace = Mono::GetStringProperty("StackTrace", exceptionClass, exception);
		std::string message = Mono::GetStringProperty("Message", exceptionClass, exception);

		HazardScriptEngine::SendDebugMessage({ Severity::Error, typeName, message + "\n\n" + stacktrace});
	}
	void HazardScriptEngine::InitializeMono()
	{
		Mono::SetDirs(m_MonoData.MonoAssemblyDir, m_MonoData.MonoConfigDir);

		Mono::Init("HazardScriptCore");
		ScriptCache::Init();

		if (m_MonoData.LoadAssembliesOnInit) {
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

		if (!m_MonoData.CoreAssembly.LoadFromSource(true)) {
			SendDebugMessage({ Severity::Critical, "Core assembly loading failed" });
			return;
		}
	}
	void HazardScriptEngine::LoadRuntimeAssembly()
	{
		if (!m_MonoData.AppAssembly.LoadFromSource(true)) {
			SendDebugMessage({ Severity::Critical, "App assembly loading failed" });
			return;
		}
		m_MonoData.BindingCallback();
	}
}