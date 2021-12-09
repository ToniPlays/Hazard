#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Hazard/Module.h"
#include "ScriptEngine.h"
#include "PublicField.h"

namespace Hazard::Scripting {

	enum class ScriptType {
		CSharpScript = 0,
		VisualScript = 1
	};

	static const char* ScriptTypeToString(ScriptType type) {
		switch (type)
		{
		case ScriptType::CSharpScript:	return "C# Script";
		case ScriptType::VisualScript:	return "Hazard Visual Script";
		}
		return "Unknown";
	}

	class ScriptEngineManager : public Module::Module 
	{
	public:
		ScriptEngineManager(ScriptEngineCreateInfo* info);
		~ScriptEngineManager();

		ScriptRegistry GetRegistry() { return m_Registry; };

		void Close() override;
		void Update() override;

		void Instantiate(ScriptType type, uint32_t handle, const std::string& moduleName);

		void OnRuntimeEnd();

		void InitEntity(ScriptType type, uint32_t handle, const std::string& moduleName);
		void ClearEntity(ScriptType type, uint32_t handle, const std::string& moduleName);
		void ReloadAll();

		bool ModuleExists(ScriptType type, const char* moduleName);

		std::unordered_map<uint32_t, ScriptField*> GetFields(ScriptType type, uint32_t handle, const std::string& moduleName);
		std::unordered_map<ScriptType, ScriptEngine*>& GetScriptEngines() { return m_ScriptEngines; };
		ScriptEngine& GetEngine(ScriptType type) { return *m_ScriptEngines[type]; }

	private:
		ScriptRegistry m_Registry;
		std::unordered_map<ScriptType, ScriptEngine*> m_ScriptEngines;
	};
}