#pragma once

#include "File.h"
#include "Severity.h"
#include <functional>


namespace HazardScript 
{
	struct ScriptMessage 
	{
		Severity Severity;
		std::string Message;
	};

	using ScriptDebugCallback = std::function<void(ScriptMessage)>;
	using BindingCallback = std::function<void()>;

	struct HazardScriptCreateInfo 
	{
		std::filesystem::path CoreAssemblyPath;
		std::filesystem::path AppAssemblyPath;

		std::filesystem::path AssemblyPath;
		std::filesystem::path ConfigPath;

		ScriptDebugCallback DebugCallback;
		BindingCallback BindingCallback;
	};
}