#pragma once

#include <hzrpch.h>
#include "Application.h"
#include "Hazard/RenderContext/RenderContext.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "HazardLoop.h"

namespace Hazard {

	void Application::SetTitle(const std::string& title)
	{
		SetTitle(title.c_str());
	}
	void Application::SetTitle(const char* title)
	{
		auto* context = GetModule<Rendering::RenderContext>();
		context->GetWindow().SetWindowTitle(title);
	}
	void Application::CreateApplicationStack(HazardCreateInfo* info)
	{
		if (info->appInfo == nullptr) 
			HZR_THROW("Must assign ApplicationCreateInfo to HazardCreateInfo");

#ifndef HZR_RELEASE
			PushModule<Logging::Logger>();
#endif // HZR_RELEASE

		if (info->renderContextInfo != nullptr) {
			PushModule<Rendering::RenderContext>().InitContext(info->renderContextInfo, info->appInfo);
		}
		if (info->rendererInfo != nullptr) {
			if (info->renderContextInfo == nullptr) HZR_THROW("Hazard RenderEngine requires valid RenderContext");
			PushModule<Rendering::RenderEngine>().InitRenderer(info->rendererInfo);
		}
	}
	void Application::Quit()
	{
		Core::HazardLoop::GetCurrent().Shutdown();
	}
}
