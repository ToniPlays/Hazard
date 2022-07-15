
#include <hzrpch.h>
#include "ScriptEngine.h"

using namespace HazardScript;

namespace Hazard 
{
	ScriptEngine::ScriptEngine(ScriptEngineCreateInfo* info) : Module("ScriptEngine")
	{
		HazardScriptCreateInfo createInfo = {};
		createInfo.CoreAssemblyPath = info->CoreAssemblyPath;
		createInfo.AppAssemblyPath = info->AppAssemblyPath;
		createInfo.AssemblyPath = info->AssemblyPath;
		createInfo.ConfigPath = info->ConfigPath;

		createInfo.DebugCallback = [](ScriptMessage message) {
			HZR_CORE_INFO(message.Message);
		};
		createInfo.BindingCallback = []() {
			HZR_CORE_INFO("We binding");
		};


		m_Engine = HazardScriptEngine::Create(&createInfo);
	}
	bool ScriptEngine::HasModule(const std::string& moduleName)
	{
		return m_Engine->GetAppAssembly().HasScript(moduleName);
	}
	Script& ScriptEngine::GetScript(const std::string& moduleName) {
		return m_Engine->GetAppAssembly().GetScript(moduleName);
	}
}