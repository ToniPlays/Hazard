#pragma once

#include "Core.h"
#include "glm/glm.hpp"
#include "Severity.h"
#include "Event.h"
#include <string>
#include <functional>

namespace Hazard
{
	struct RenderMessage 
	{
		Severity Severity;
		std::string Message;
	};

	using RendererMessageCallback = void(*)(RenderMessage);
	using EventCallback = std::function<void(Event&)>;

	enum class RenderAPI {
		Auto = 0,
		OpenGL,
		Vulkan,
		DX11,
		DX12,
		Metal
	};

	std::string RenderAPIToString(RenderAPI api);


	struct HazardRendererAppInfo 
	{
		std::string AppName;
		std::string BuildVersion;

		const char** pIcons;
		uint32_t IconCount = 0;
		RendererMessageCallback MessageCallback = nullptr;
		EventCallback EventCallback = nullptr;
	};

	struct HazardWindowCreateInfo
	{
		std::string Title;

		bool FullScreen = false;
		bool Maximized = false;
		bool Resizable = true;
		bool Decorated = true;

		glm::vec4 Color = { 0.0f, 0.0f, 0.0f, 1.0f };

		uint32_t Width = 0;
		uint32_t Height = 0;
	};

	struct HazardRendererCreateInfo
	{
		RenderAPI Renderer = RenderAPI::Auto;

		uint32_t WindowCount = 0;
		HazardWindowCreateInfo* pWindows;

		uint32_t ImagesInFlight = 0;
		bool Logging = true;
		bool VSync = false;
		

		HazardRendererAppInfo* pAppInfo = nullptr;
	};
	
}