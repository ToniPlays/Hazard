
#pragma once
#include <hzrpch.h>
#include "DisplayBindings.h"
#include "Hazard/RenderContext/RenderContext.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

namespace Hazard::Scripting::CSharp::Bindings {

	BindMap DisplayBindings::GetBindings()
	{
		BindMap result;
		result.reserve(6);
		result.emplace_back("Hazard.Display::Display_Width_Native", (void*) GetDisplayWidth);
		result.emplace_back("Hazard.Display::Display_Height_Native", (void*) GetDisplayHeight);
		result.emplace_back("Hazard.Display::Display_IsFullscreen_Native", (void*) GetDisplayFullscreen);
		result.emplace_back("Hazard.Display::Display_SetFullscreen_Native", (void*) SetDisplayFullscreen);
		result.emplace_back("Hazard.Display::Display_IsVsync_Native", (void*) GetDisplayVSync);
		result.emplace_back("Hazard.Display::Display_SetVsync_Native", (void*) SetDisplayVsync);
		return result;
	}
	float DisplayBindings::GetDisplayWidth()
	{
		return Rendering::RenderContextCommand::GetWindowWidth();
	}
	float DisplayBindings::GetDisplayHeight()
	{
		return Rendering::RenderContextCommand::GetWindowHeight();
	}
	bool DisplayBindings::GetDisplayFullscreen()
	{
		return Rendering::RenderContextCommand::IsFullscreen();
	}
	void DisplayBindings::SetDisplayFullscreen(MonoObject* obj)
	{
		return Rendering::RenderContextCommand::SetFullscreen(obj);
	}
	bool DisplayBindings::GetDisplayVSync()
	{
		return Rendering::RenderContextCommand::IsVsync();
	}
	void DisplayBindings::SetDisplayVsync(MonoObject* obj)
	{
		Rendering::RenderContextCommand::SetVsync(obj);
	}
}