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
		
		void Init() override;
		void Update() override;

		bool HasModule(const std::string& moduleName);
		HazardScript::ScriptMetadata& GetScript(const std::string& moduleName);
		void SendDebugMessage(const HazardScript::ScriptMessage& message);

		void RegisterInternalCall(const std::string& signature, void* method) {
#ifdef HZR_INCLUDE_MONO
			m_Engine->RegisterInternalCall(signature, method);
#endif
		}
		template<typename T>
		void RegisterScriptGlue() 
		{
			static_assert(std::is_base_of<IScriptGlue, T>::value, "Cannot use this type");
			T* glue = hnew T();
			m_ScriptGlue.push_back((IScriptGlue*)glue);
		}

		void ReloadAssemblies();
		void SetDebugCallback(ScriptMessageCallback callback);
		void InitializeComponent(Entity& entity, ScriptComponent& component);

	private:
		ScriptEngineCreateInfo m_Info;
#ifdef HZR_INCLUDE_MONO
		HazardScript::HazardScriptEngine* m_Engine;
#endif
		std::vector<HazardScript::ScriptMessage> m_QueuedMessages;
		std::vector<IScriptGlue*> m_ScriptGlue;
		ScriptMessageCallback m_MessageCallback;
	};
}
