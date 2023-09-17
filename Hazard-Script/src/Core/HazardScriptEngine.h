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
		void RunGarbageCollector();
		bool LoadAssembly(Ref<ScriptAssembly> assembly);
	
		Ref<ScriptAssembly> GetLoadedAssembly(const std::string& name)
		{
			for (auto& assembly : GetAssemblies())
				if (assembly->GetName() == name)
					return assembly;

			return nullptr;
		}
		const std::vector<Ref<ScriptAssembly>>& GetAssemblies();

	public:
		static HazardScriptEngine* Create(HazardScriptCreateInfo* info);
		static void SendDebugMessage(ScriptMessage message);
		static CoralData& GetCoralData() { return s_Instance->m_CoralData; }

	private:
		HazardScriptEngine(HazardScriptCreateInfo* info);

		void InitializeCoralHost();
		void ReloadAppScripts();

	private:
		Coral::HostInstance m_HostInstance;
		Coral::AssemblyLoadContext m_LoadContext;

		ScriptDebugCallback m_DebugCallback;
		CoralData m_CoralData;
		std::vector<Ref<ScriptAssembly>> m_Assemblies;

		inline static HazardScriptEngine* s_Instance;

	};
}
