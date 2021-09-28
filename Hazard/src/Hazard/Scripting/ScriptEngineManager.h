#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Hazard/Module.h"
#include "ScriptEngine.h"
#include "PublicField.h"

namespace Hazard::Scripting {

	enum ScriptType {
		CSharpScript = 0,
		VisualScript
	};

	class ScriptEngineManager : public Module::Module 
	{
	public:
		ScriptEngineManager(ScriptEngineCreateInfo* info);

		void Close() override;
		void Update() override;

		void Instantiate(ScriptType type, uint32_t entityID, const std::string& moduleName);

		void OnRuntimeEnd();

		void InitEntity(ScriptType type, uint32_t entityID, const std::string& moduleName);
		void ClearEntity(ScriptType type, uint32_t entityID, const std::string& moduleName);
		void ReloadAll();

		bool ModuleExists(ScriptType type, const char* moduleName);

		std::unordered_map<std::string, PublicField*> GetPublicFields(ScriptType type, uint32_t entity, const std::string& moduleName);
		std::unordered_map<ScriptType, ScriptEngine*>& GetScriptEngines() { return m_ScriptEngines; };
		ScriptEngine& GetEngine(ScriptType type) { return *m_ScriptEngines[type]; }

	private:
		std::unordered_map<ScriptType, ScriptEngine*> m_ScriptEngines;
	};
}