#pragma once

#include "Hazard.h"

namespace Hazard
{
	static bool Display_IsFullscreen_Native()
	{
		HazardRenderer::Window& window = Application::GetModule<RenderContextManager>().GetWindow();
		return window.IsFullscreen();
	}
	static void Display_SetFullscreen_Native(bool fullscreen)
	{
		HazardRenderer::Window& window = Application::GetModule<RenderContextManager>().GetWindow();
		window.SetFullscreen(fullscreen);
	}
	static bool Display_IsVsync_Native()
	{
		HazardRenderer::Window& window = Application::GetModule<RenderContextManager>().GetWindow();
		return window.IsVSync();
	}
	static void Display_SetVsync_Native(bool vsync)
	{
		HazardRenderer::Window& window = Application::GetModule<RenderContextManager>().GetWindow();
		window.SetVSync(vsync);
	}
}