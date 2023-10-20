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

		Ref<HazardScript::ScriptAssembly> GetLoadedAssembly(const std::string& name)
		{
			for (auto& assembly : GetAssemblies())
				if (assembly->GetName() == name)
					return assembly;
			return nullptr;
		};

		void ReloadAssemblies();
		void SetDebugCallback(ScriptMessageCallback callback);
		void InitializeComponent(Ref<World> targetWorld, const Entity& entity);
		void RemoveComponent(Ref<World> targetWorld, const Entity& entity);

		static Ref<World> GetTargetWorldForEntity(UID entity)
		{
			auto& engine = Application::GetModule<ScriptEngine>();
			HZR_ASSERT(engine.m_Instances.contains(entity), "Entity not found");
			return engine.m_WorldContext[engine.m_Instances[entity]];
		}

		const std::vector<Ref<HazardScript::ScriptAssembly>>& GetAssemblies();
	private:

		ScriptEngineCreateInfo m_Info;
		HazardScript::HazardScriptEngine* m_Engine;
		std::vector<HazardScript::ScriptMessage> m_QueuedMessages;
		std::unordered_map<UID, UID> m_Instances;
		std::unordered_map<UID, Ref<World>> m_WorldContext;

		std::unordered_map<HazardScript::ScriptAssembly*, std::vector<IScriptGlue*>> m_ScriptGlue;
		ScriptMessageCallback m_MessageCallback;
	};
}
