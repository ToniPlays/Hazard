#pragma once

#include <glm/glm.hpp>
#include "Color.h"
#include "HazardRendererCore.h"

namespace HazardScript {
	struct ScriptMessage;
}

namespace Hazard {

	class ScriptEngine;

	using ScriptBindCallback = std::function<void(ScriptEngine*)>;
	using ScriptMessageCallback = std::function<void(HazardScript::ScriptMessage)>;

	struct ApplicationCreateInfo 
	{
		std::string AppName;
		std::string BuildVersion;
		bool Logging = false;
	};
	struct AudioEngineCreateInfo
	{

	};
	struct EntityComponentCreateInfo 
	{
		std::filesystem::path StartupFile = "";
	};
	
	struct ScriptEngineCreateInfo 
	{
		std::string AppAssemblyPath;
		std::string CoreAssemblyPath;
		std::string AssemblyPath;
		std::string ConfigPath;

		ScriptMessageCallback MessageCallback;
	};

	struct HazardCreateInfo 
	{
		ApplicationCreateInfo* AppInfo = nullptr;
		HazardRendererCreateInfo* RendererInfo = nullptr;
		ScriptEngineCreateInfo* ScriptEngineInfo = nullptr;
		AudioEngineCreateInfo* AudioEngine = nullptr;
		EntityComponentCreateInfo* EntityComponent = nullptr;
	};
}