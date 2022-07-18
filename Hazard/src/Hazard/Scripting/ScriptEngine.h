#pragma once

#include "Hazard/Module.h"
#include "HazardScript.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Hazard/ECS/Entity.h"

namespace Hazard 
{
	class ScriptEngine : public Module 
	{
	public:
		ScriptEngine(ScriptEngineCreateInfo* info);
		~ScriptEngine() = default;
		
		bool HasModule(const std::string& moduleName);
		HazardScript::Script& GetScript(const std::string& moduleName);
		void SendDebugMessage(const HazardScript::ScriptMessage& message);

		void RegisterInternalCall(const std::string& signature, void* method) {
			m_Engine->RegisterInternalCall(signature, method);
		}
		void AddBindingCallback(ScriptBindCallback callback) {
			m_BindCallbacks.push_back(callback);
		}

		void SetDebugCallback(ScriptMessageCallback callback);
		void InitializeComponent(Entity& entity, ScriptComponent& component);

	private:
		HazardScript::HazardScriptEngine* m_Engine;
		std::vector<HazardScript::ScriptMessage> m_QueuedMessages;
		std::vector<ScriptBindCallback> m_BindCallbacks;
		ScriptMessageCallback m_MessageCallback;
	};
}