#pragma once
#include <hzrpch.h>
#include "ApplicationBindings.h"
#include "Hazard/RenderContext/RenderContext.h"

namespace Hazard::Scripting::Bindings {

#pragma region Display 
	static float GetDisplayWidth() 
	{
		return Application::GetModule<Rendering::RenderContext>().GetWindow().GetWidth();
	}
	static float GetDisplayHeight() 
	{
		return Application::GetModule<Rendering::RenderContext>().GetWindow().GetHeight();
	}
	static bool GetDisplayFullscreen()
	{
		return Application::GetModule<Rendering::RenderContext>().GetWindow().IsFullscreen();
	}
	static void SetDisplayFullscreen(MonoObject* obj)
	{
		Application::GetModule<Rendering::RenderContext>().GetWindow().SetFullscreen(obj);
	}
	static bool GetDisplayVsync()
	{
		return Application::GetModule<Rendering::RenderContext>().GetWindow().IsVSync();
	}
	static void SetDisplayVsync(MonoObject* obj)
	{
		Application::GetModule<Rendering::RenderContext>().GetWindow().SetVSync(obj);
	}
#pragma endregion

	BindMap ApplicationBindings::GetBindings()
	{
		BindMap results;
		//Application class
		{
			results.emplace_back("Hazard.Application::Application_Quit_Native", Application::Quit);
		}
		//Screen class
		{
			results.emplace_back("Hazard.Display::Display_Width_Native", GetDisplayWidth);
			results.emplace_back("Hazard.Display::Display_Height_Native", GetDisplayHeight);
			results.emplace_back("Hazard.Display::Display_IsFullscreen_Native", GetDisplayFullscreen);
			results.emplace_back("Hazard.Display::Display_SetFullscreen_Native", SetDisplayFullscreen);

			results.emplace_back("Hazard.Display::Display_IsVsync_Native", GetDisplayVsync);
			results.emplace_back("Hazard.Display::Display_SetVsync_Native", SetDisplayVsync);
		}
		return results;
	}
}
