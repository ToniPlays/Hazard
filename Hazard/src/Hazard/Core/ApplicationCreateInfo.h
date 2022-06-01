#pragma once

#include <glm/glm.hpp>
#include "Color.h"

namespace Hazard {

	struct ApplicationCreateInfo 
	{
		std::string AppName;
		std::string BuildVersion;

		const char** Icons = nullptr;
		uint32_t IconCount = 0;

		bool Logging = false;
	};
	struct AudioEngineCreateInfo
	{

	};
	struct EntityComponentCreateInfo 
	{
		std::string StartupFile = "";
	};
	
	struct RenderEngineCreateInfo 
	{
		uint32_t MaxQuadCount = 35000;
		uint32_t SamplerCount = 0;
		bool Enabled = true;
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
		//RenderContexCreateInfo* RenderContextInfo = nullptr;
		RenderEngineCreateInfo* RendererInfo = nullptr;
		ScriptEngineCreateInfo* ScriptEngineInfo = nullptr;
		AudioEngineCreateInfo* AudioEngine = nullptr;
		EntityComponentCreateInfo* EntityComponent = nullptr;
	};
}