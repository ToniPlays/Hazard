#pragma once
#include <hzrpch.h>
#include "Application.h"
#include "Hazard/ModuleHandler.h"
#include "Hazard/Rendering/Core/RenderEngine.h"

namespace Hazard {
	void Application::PushModule(Module* module)
	{
		ModuleHandler::PushModule(module);
	}
	void Application::SetTitle(const char* title)
	{
		HZR_CORE_INFO("Title not able to be set");
		ModuleHandler::GetModule<RenderEngine>()->SetTitle(title);
	}
	Application& Application::GetApplication()
	{
		return *HazardLoop::instance->current;
	}
}
