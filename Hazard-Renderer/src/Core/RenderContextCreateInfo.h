#pragma once

#include "Core/Core.h"
#include "glm/glm.hpp"
#include <string>

namespace HazardRenderer 
{
	enum class RenderAPI {
		Auto = 0,
		OpenGL,
		Vulkan,
		DX11,
		DX12,
		Metal
	};

	struct HazardRendererAppInfo 
	{
		std::string AppName;
		std::string BuildVersion;

		const char** Icons;
		uint32_t IconCount = 0;
	};

	struct HazardRendererCreateInfo
	{
		RenderAPI Renderer = RenderAPI::Auto;
		bool FullScreen = false;
		bool Maximized = false;
		bool Resizable = true;
		bool VSync = false;
		bool Decorated = true;
		bool Logging = true;

		glm::vec4 Color = { 0.0f, 0.0f, 0.0f, 1.0f };

		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t ImagesInFlight = 0;

		HazardRendererAppInfo* AppInfo = nullptr;
	};
}