#pragma once

#include "HazardScriptCore.h"
#include "Metadata/ScriptAssembly.h"
#include "Coral/HostInstance.hpp"

#include "Callback.h"

namespace HazardScript 
{
	struct CoralData
    {
		std::filesystem::path CoralDirectory;
		
		Ref<ScriptAssembly> CoreAssembly;
		Ref<ScriptAssembly> AppAssembly;

		std::function<void(Ref<ScriptAssembly>)> BindingCallback;
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

	private:
		Coral::HostInstance m_HostInstance;
		Coral::AssemblyLoadContext m_LoadContext;

		Callback<void(const ScriptMessage&)> m_DebugCallbacks;

		CoralData m_CoralData;
		std::vector<Ref<ScriptAssembly>> m_Assemblies;

		std::mutex m_ReloadMutex;
		inline static HazardScriptEngine* s_Instance;

	};
}
