#pragma once

#include "HazardScriptCore.h"
#include "Metadata/ScriptAssembly.h"

namespace HazardScript 
{
	struct MonoData {
		MonoDomain* Domain;

		std::filesystem::path MonoAssemblyDir;
		std::filesystem::path MonoConfigDir;
		
		ScriptAssembly CoreAssembly;
		ScriptAssembly AppAssembly;

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

		ScriptAssembly& GetAppAssembly() { return m_MonoData.AppAssembly; }
	
	public:
		static HazardScriptEngine* Create(HazardScriptCreateInfo* info);
		static void SendDebugMessage(ScriptMessage message);
		static std::vector<ScriptAssembly*> GetAssemblies();
		static void CheckError(MonoObject* exception, MonoObject* result, MonoMethod* method);
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