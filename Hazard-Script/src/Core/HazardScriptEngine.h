#pragma once


#include "HazardScriptCore.h"
#include "Metadata/ScriptAssembly.h"
#include "Coral/HostInstance.hpp"

namespace HazardScript 
{

	struct CoralData
    {
		std::filesystem::path CoralDirectory;
		
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

		Ref<ScriptAssembly> GetAppAssembly() { return m_CoralData.AppAssembly; }
	
	public:
		static HazardScriptEngine* Create(HazardScriptCreateInfo* info);
		static void SendDebugMessage(ScriptMessage message);
		static std::vector<Ref<ScriptAssembly>> GetAssemblies();
		static CoralData& GetCoralData() { return s_Instance->m_CoralData; }

	private:
		HazardScriptEngine(HazardScriptCreateInfo* info);

		void InitializeCoralHost();
		void LoadCoreAssebly();
		void LoadRuntimeAssembly();
		void ReloadAppScripts();

	private:

		Coral::HostInstance m_HostInstance;

		ScriptDebugCallback m_DebugCallback;
		CoralData m_CoralData;

		inline static HazardScriptEngine* s_Instance;

	};
}
