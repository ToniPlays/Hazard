
#pragma once
#include <hzrpch.h>
#include "DisplayBindings.h"
#include "Hazard/RenderContext/RenderContext.h"

namespace Hazard::Scripting::CSharp::Bindings {

	BindMap DisplayBindings::GetBindings()
	{
		BindMap result;
		result.reserve(6);
		result.emplace_back("Hazard.Display::Display_Width_Native", GetDisplayWidth);
		result.emplace_back("Hazard.Display::Display_Height_Native", GetDisplayHeight);
		result.emplace_back("Hazard.Display::Display_IsFullscreen_Native", GetDisplayFullscreen);
		result.emplace_back("Hazard.Display::Display_SetFullscreen_Native", SetDisplayFullscreen);
		result.emplace_back("Hazard.Display::Display_IsVsync_Native", GetDisplayVSync);
		result.emplace_back("Hazard.Display::Display_SetVsync_Native", SetDisplayVsync);
		return result;
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
		Application::GetModule<Rendering::RenderContext>().GetWindow().SetFullscreen(obj);
	}
	void DisplayBindings::SetDisplayVsync(MonoObject* obj)
	{
		Application::GetModule<Rendering::RenderContext>().GetWindow().SetVSync(obj);
	}
}