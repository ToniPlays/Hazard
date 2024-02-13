#pragma once

#include "Hazard.h"
#include "Coral/Array.hpp"

namespace Hazard
{
	static bool Display_IsFullscreen_Native()
	{
		return false;
	}

	static void Display_SetFullscreen_Native(bool fullscreen)
	{

	}

	static bool Display_IsVsync_Native()
	{
		return false;
	}

	static void Display_SetVsync_Native(bool vsync)
	{

	}

	static Resolution Display_GetResolution_Native()
	{

		return Resolution();
	}

	static Coral::Array<HazardRenderer::Resolution> Display_GetResolutions_Native()
	{
		return Coral::Array<Resolution>::New({});
	}

	static void Display_SetResolution_Native(Resolution resolution)
	{

	}
}