#pragma once

#include <hzrpch.h>
#include "DisplayBindings.h"

namespace Hazard::Scripting::Bindings {

	BindMap DisplayBindings::GetBindings()
	{
		BindMap results;
		results.emplace_back("Hazard.Display::Display_Width_Native", GetDisplayWidth);
		results.emplace_back("Hazard.Display::Display_Height_Native", GetDisplayHeight);
		results.emplace_back("Hazard.Display::Display_IsFullscreen_Native", GetDisplayFullscreen);
		results.emplace_back("Hazard.Display::Display_SetFullscreen_Native", SetDisplayFullscreen);

		results.emplace_back("Hazard.Display::Display_IsVsync_Native", GetDisplayVSync);
		results.emplace_back("Hazard.Display::Display_SetVsync_Native", SetDisplayVsync);
		return results;
	}
	float DisplayBindings::GetDisplayWidth()
	{
		return Application::GetModule<Rendering::RenderContext>().GetWindow().GetWidth();
	}
	float DisplayBindings::GetDisplayHeight()
	{
		return Application::GetModule<Rendering::RenderContext>().GetWindow().GetHeight();
	}
	bool DisplayBindings::GetDisplayFullscreen()
	{
		return Application::GetModule<Rendering::RenderContext>().GetWindow().IsFullscreen();
	}
	bool DisplayBindings::GetDisplayVSync()
	{
		return Application::GetModule<Rendering::RenderContext>().GetWindow().IsVSync();
	}
	void DisplayBindings::SetDisplayFullscreen(MonoObject* obj)
	{
		Application::GetModule<Rendering::RenderContext>().GetWindow().SetFullscreen(true);
	}
	void DisplayBindings::SetDisplayVsync(MonoObject* obj)
	{
		Application::GetModule<Rendering::RenderContext>().GetWindow().SetVSync(true);
	}
}
