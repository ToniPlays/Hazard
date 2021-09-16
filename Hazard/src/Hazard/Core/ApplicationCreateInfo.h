#pragma once

#include <glm/glm.hpp>

namespace Hazard {

	enum class RenderAPI {
		Auto = 0,
		OpenGL,
		Vulkan,
		DX11,
		DX12
	};

	struct ApplicationCreateInfo 
	{
		std::string appName;
		std::string buildVersion;

		std::string* icons = nullptr;
		uint32_t iconCount = 0;

		bool logging = false;
	};
	struct AudioEngineCreateInfo
	{

	};
	struct EntityComponentCreateInfo 
	{
		std::string startupFile = "";
	};
	struct RenderContexCreateInfo 
	{
		RenderAPI renderer = RenderAPI::Auto;
		bool fullScreen = false;
		bool maximized = false;
		bool resizable = true;
		bool VSync = false;
		bool decorated = true;
		
		glm::vec3 color = { 0, 0, 0 };

		uint32_t width = 0;
		uint32_t height = 0;
	};
	struct RenderEngineCreateInfo 
	{
		std::string shaderSourcePath;
		std::string shaderCompilePath;

		uint32_t maxQuadCount = 35000;
		uint32_t samplerCount = 0;
	};
	struct ScriptEngineCreateInfo 
	{
		std::string appAssemblyPath;
		std::string coreAssemblyPath;
		std::string monoDirectory;
		bool enable = true;
	};

	struct HazardCreateInfo 
	{
		ApplicationCreateInfo* appInfo = nullptr;
		RenderContexCreateInfo* renderContextInfo = nullptr;
		RenderEngineCreateInfo* rendererInfo = nullptr;
		ScriptEngineCreateInfo* scriptEngineInfo = nullptr;
		AudioEngineCreateInfo* audioEngine = nullptr;
		EntityComponentCreateInfo* entityComponent = nullptr;
	};
}