#pragma once

#include <glm/glm.hpp>
#include "Hazard/Math/Color.h"

namespace Hazard {

	enum class RenderAPI {
		Auto = 0,
		OpenGL,
		Vulkan,
		DX11,
		DX12,
		Metal
	};
	enum class Severity
	{
		Debug,
		Trace,
		Info,
		Warning,
		Error,
		Critical
	};

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
	struct RenderContexCreateInfo 
	{
		RenderAPI Renderer = RenderAPI::Auto;
		bool FullScreen = false;
		bool Maximized = false;
		bool Resizable = true;
		bool VSync = false;
		bool Decorated = true;
		
		Color Color = { 0.0f, 0.0f, 0.0f, 1.0f };

		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t ImagesInFlight = 2;
	};
	struct RenderEngineCreateInfo 
	{
		uint32_t MaxQuadCount = 35000;
		uint32_t SamplerCount = 0;
	};
	struct ScriptEngineCreateInfo 
	{
		const char* AppAssemblyPath;
		const char* CoreAssemblyPath;
		const char* MonoDirectoryEtc;
		const char* MonoDirectoryLib;
		bool Enable = true;
	};

	struct HazardCreateInfo 
	{
		ApplicationCreateInfo* AppInfo = nullptr;
		RenderContexCreateInfo* RenderContextInfo = nullptr;
		RenderEngineCreateInfo* RendererInfo = nullptr;
		ScriptEngineCreateInfo* ScriptEngineInfo = nullptr;
		AudioEngineCreateInfo* AudioEngine = nullptr;
		EntityComponentCreateInfo* EntityComponent = nullptr;
	};
}