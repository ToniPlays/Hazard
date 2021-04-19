#pragma once

#include "Hazard/Module/Module.h"
#include "ScriptEngine.h"
#include "Hazard/Core/Core.h"

namespace Hazard::Scripting {

	enum ScriptType {
		CSharpScript = 0,
		VisualScript
	};

	class ScriptEngineManager : public Module::Module 
	{
	public:
		ScriptEngineManager();

		void Init() override;
		void Close() override;

		void Update() override;

		void BeginRuntime();
		void EndRuntime();
		void InitEntity(uint32_t entityID, std::string moduleName, ScriptType type);
		void ClearEntity(uint32_t entityID, std::string moduleName, ScriptType type);
		void ReloadAll();

		bool ModuleExists(ScriptType type, const char* moduleName);
		ScriptData GetData(ScriptType type, uint32_t entity, std::string moduleName);

		std::unordered_map<ScriptType, ScriptEngine*>& GetScriptEngines() { return scriptEngines; };

	private:
		std::unordered_map<ScriptType, ScriptEngine*> scriptEngines;
	};
}