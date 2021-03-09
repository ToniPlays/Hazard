#pragma once

#include <hzrpch.h>
#include "Application.h"
#include "Hazard/RenderContext/RenderContext.h"
#include "HazardLoop.h"

namespace Hazard {

	void Application::SetTitle(std::string title)
	{
		SetTitle(title.c_str());
	}

	void Application::SetTitle(const char* title)
	{
		auto& context = GetModule<Rendering::RenderContext>();
		context.GetWindow().SetWindowTitle(title);
	}

	void Application::SetWindowIcon(const char* windowIcon, const char* barIcon)
	{
		auto& context = GetModule<Rendering::RenderContext>();
		context.GetWindow().SetWindowIcon(windowIcon, barIcon);
	}

	void Application::Quit()
	{
		Core::HazardLoop::GetCurrent().Shutdown();
	}
}
