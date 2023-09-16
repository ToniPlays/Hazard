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
		uint32_t ThreadCount = std::thread::hardware_concurrency();
	};

	struct RenderContextCreateInfo 
	{
		HazardRenderer::RenderAPI Renderer = HazardRenderer::RenderAPI::Auto;
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool VSync = true;
	};
	struct RendererCreateInfo 
	{
		bool some = true;
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
		uint32_t UpdateOrder = 0;

		std::string AppAssemblyPath;
		std::string CoreAssemblyPath;
		std::string CoralDirectory;

		ScriptMessageCallback MessageCallback;
	};

	struct HazardCreateInfo 
	{
		ApplicationCreateInfo* AppInfo = nullptr;
		RenderContextCreateInfo* RenderContextInfo = nullptr;
		RendererCreateInfo* RendererInfo = nullptr;
		ScriptEngineCreateInfo* ScriptEngineInfo = nullptr;
		AudioEngineCreateInfo* AudioEngine = nullptr;
		EntityComponentCreateInfo* EntityComponent = nullptr;
	};
}