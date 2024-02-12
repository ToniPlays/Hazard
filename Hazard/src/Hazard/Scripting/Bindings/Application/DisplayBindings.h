#pragma once

#include "Hazard.h"
#include "Coral/Array.hpp"

namespace Hazard
{
	static bool Display_IsFullscreen_Native()
	{
		auto& window = Application::GetModule<RenderContextManager>().GetWindow();
		return window.IsFullscreen();
	}

	static void Display_SetFullscreen_Native(bool fullscreen)
	{
		auto& window = Application::GetModule<RenderContextManager>().GetWindow();
		window.SetFullscreen(fullscreen);
	}

	static bool Display_IsVsync_Native()
	{
		auto& window = Application::GetModule<RenderContextManager>().GetWindow();
		return window.IsVSync();
	}

	static void Display_SetVsync_Native(bool vsync)
	{
		auto& window = Application::GetModule<RenderContextManager>().GetWindow();
		window.SetVSync(vsync);
	}

	static Resolution Display_GetResolution_Native()
	{
		auto& window = Application::GetModule<RenderContextManager>().GetWindow();

		Resolution resolution = {
			.Width = window.GetWidth(),
			.Height = window.GetHeight(),
			.RefreshRate = window.GetWindowInfo().RefreshRate,
		};

		return resolution;
	}

	static Coral::Array<HazardRenderer::Resolution> Display_GetResolutions_Native()
	{
		auto& window = Application::GetModule<RenderContextManager>().GetWindow();
		auto resolutions = window.GetAvailableResolutions();
		return Coral::Array<Resolution>::New(resolutions);
	}

	static void Display_SetResolution_Native(Resolution resolution)
	{
		auto& window = Application::GetModule<RenderContextManager>().GetWindow();
		window.SetResolution(resolution);
	}
	
}