#pragma once

#include "HazardScriptCore.h"
#include "ScriptAssembly.h"

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

		ScriptAssembly& GetAppAssembly() { return m_MonoData.AppAssembly; }
	
	public:
		static HazardScriptEngine* Create(HazardScriptCreateInfo* info);
		static void SendDebugMessage(ScriptMessage message);
		static std::vector<ScriptAssembly*> GetAssemblies();
		static void CheckError(MonoObject* exception, MonoObject* result, MonoMethod* method);

	private:
		HazardScriptEngine(HazardScriptCreateInfo* info);

		void InitializeMono();
		void LoadCoreAssebly();
		void LoadRuntimeAssembly();

	private:

		ScriptDebugCallback m_DebugCallback;
		MonoData m_MonoData;

		inline static HazardScriptEngine* s_Instance;

	};
}