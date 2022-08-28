#pragma once

#include <glm/glm.hpp>
#include "Color.h"
#include "Core.h"
#include "Hazard/Assets/Asset.h"
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

	struct RenderContextCreateInfo 
	{
		RenderAPI Renderer = RenderAPI::Auto;
		bool VSync = true;
		//std::thread* Thread;
	};

	struct AudioEngineCreateInfo
	{
		//std::thread* Thread;
	};
	struct EntityComponentCreateInfo 
	{
		//std::thread* Thread;

		std::filesystem::path StartupFile = "";
	};
	
	struct ScriptEngineCreateInfo 
	{
		uint32_t UpdateOrder = 0;
		//std::thread* Thread;

		std::string AppAssemblyPath;
		std::string CoreAssemblyPath;
		std::string AssemblyPath;
		std::string ConfigPath;

		ScriptMessageCallback MessageCallback;
	};

	struct HazardCreateInfo 
	{
		ApplicationCreateInfo* AppInfo = nullptr;
		RenderContextCreateInfo* RenderContextInfo = nullptr;
		ScriptEngineCreateInfo* ScriptEngineInfo = nullptr;
		AudioEngineCreateInfo* AudioEngine = nullptr;
		EntityComponentCreateInfo* EntityComponent = nullptr;
	};
}