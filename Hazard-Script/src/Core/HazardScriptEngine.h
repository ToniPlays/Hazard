#pragma once


#include "HazardScriptCore.h"
#include "Metadata/ScriptAssembly.h"

namespace HazardScript 
{

	struct MonoData
    {
#ifdef HZR_INCLUDE_MONO
		MonoDomain* Domain;
#endif

		std::filesystem::path MonoAssemblyDir;
		std::filesystem::path MonoConfigDir;
		
		Ref<ScriptAssembly> CoreAssembly;
		Ref<ScriptAssembly> AppAssembly;

		BindingCallback BindingCallback;
		bool LoadAssembliesOnInit = true;
	};

	class HazardScriptEngine 
	{
	public:
		HazardScriptEngine() = delete;
		~HazardScriptEngine() {};

		void Reload();
		void RegisterInternalCall(const std::string& signature, void* function);
		void RunGarbageCollector();

		Ref<ScriptAssembly> GetAppAssembly() { return m_MonoData.AppAssembly; }
	
	public:
		static HazardScriptEngine* Create(HazardScriptCreateInfo* info);
		static void SendDebugMessage(ScriptMessage message);
		static std::vector<Ref<ScriptAssembly>> GetAssemblies();
        
#ifdef HZR_INCLUDE_MONO
		static void CheckError(MonoObject* exception, MonoObject* result, MonoMethod* method);
#endif
		static MonoData& GetMonoData() { return s_Instance->m_MonoData; }

	private:
		HazardScriptEngine(HazardScriptCreateInfo* info);

		void InitializeMono();
		void LoadCoreAssebly();
		void LoadRuntimeAssembly();
		void ReloadAppScripts();

	private:
		ScriptDebugCallback m_DebugCallback;
		MonoData m_MonoData;

		inline static HazardScriptEngine* s_Instance;

	};
}
