#pragma once

namespace Hazard {

	enum class RenderAPI {
		Auto = 0,
		OpenGL,
		Vulkan,
		DX11,
		DX12
	};

	struct ApplicationCreateInfo {
		std::string appName;
		std::string buildVersion;

		std::string* icons;
		uint32_t iconCount = 0;

		bool logging = false;
	};
	struct RenderContexCreateInfo 
	{
		RenderAPI renderer = RenderAPI::Auto;
		bool fullScreen = false;

		uint32_t width = 0;
		uint32_t height = 0;
	};
	struct RenderEngineCreateInfo 
	{
		uint32_t maxQuadCount = 35000;
		uint32_t samplerCount = 0;
	};

	struct HazardCreateInfo {
		ApplicationCreateInfo* appInfo = nullptr;
		RenderContexCreateInfo* renderContextInfo = nullptr;
		RenderEngineCreateInfo* rendererInfo = nullptr;
	};
}