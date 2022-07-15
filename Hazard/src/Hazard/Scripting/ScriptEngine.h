#pragma once

#include "Hazard/Module.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "HazardScript.h"

namespace Hazard 
{
	class ScriptEngine : public Module 
	{
	public:
		ScriptEngine(ScriptEngineCreateInfo* info);
		~ScriptEngine() = default;

		bool HasModule(const std::string& moduleName);
		HazardScript::Script& GetScript(const std::string& moduleName);

	private:
		HazardScript::HazardScriptEngine* m_Engine;
	};
}