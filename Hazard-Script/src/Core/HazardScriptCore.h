#pragma once

#include "File.h"
#include "Severity.h"
#include <functional>

#include "Metadata/ScriptAssembly.h"

namespace HazardScript 
{
	struct ScriptMessage 
	{
		Severity Severity;
		std::string Message;
		std::string StackTrace;
	};

	struct HazardScriptCreateInfo 
	{
		std::filesystem::path CoreAssemblyPath;
		std::filesystem::path AppAssemblyPath;

		std::filesystem::path CoralDirectory;

		std::function<void(ScriptMessage)> DebugCallback;
		std::function<void(Ref<ScriptAssembly>)> BindingCallback;

		bool LoadAssebmlies = true;
	};
}