#pragma once

#include <glm/glm.hpp>
#include "Color.h"
#include "HazardRendererCore.h"

namespace Hazard {

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
		std::string StartupFile = "";
	};
	
	struct ScriptEngineCreateInfo 
	{
		std::string AppAssemblyPath;
		std::string CoreAssemblyPath;
		std::string MonoDirectoryEtc;
		std::string MonoDirectoryLib;
		bool Enable = true;
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