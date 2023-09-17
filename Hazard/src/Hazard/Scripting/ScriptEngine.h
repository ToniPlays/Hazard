#pragma once

#include "Hazard/Module.h"
#include "HazardScript.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Hazard/ECS/Entity.h"
#include "IScriptGlue.h"

namespace Hazard 
{
	class ScriptEngine : public Module 
	{
	public:
		ScriptEngine(ScriptEngineCreateInfo* info);
		~ScriptEngine() = default;
		
		void PreInit() override;
		void Update() override;

		bool FindModule(const std::string& moduleName);

		HazardScript::ScriptMetadata GetScript(const std::string& moduleName);
		void SendDebugMessage(const HazardScript::ScriptMessage& message);

		template<typename T>
		void RegisterScriptGlueFor(Ref<HazardScript::ScriptAssembly> assembly)
		{
			static_assert(std::is_base_of<IScriptGlue, T>::value, "Cannot use this type");
			T* glue = hnew T();
			m_ScriptGlue[assembly.Raw()].push_back((IScriptGlue*)glue);
		}

		std::vector<std::string> GetLoadedAssemblyNames() { return std::vector<std::string>(); };

		void ReloadAssemblies();
		void SetDebugCallback(ScriptMessageCallback callback);
		void InitializeComponent(Entity& entity, ScriptComponent& component);

		const std::vector<Ref<HazardScript::ScriptAssembly>>& GetAssemblies();

	private:
		ScriptEngineCreateInfo m_Info;
		HazardScript::HazardScriptEngine* m_Engine;
		std::vector<HazardScript::ScriptMessage> m_QueuedMessages;

		std::unordered_map<HazardScript::ScriptAssembly*, std::vector<IScriptGlue*>> m_ScriptGlue;
		ScriptMessageCallback m_MessageCallback;
	};
}
