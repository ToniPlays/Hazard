#pragma once
#include <hzrpch.h>
#include "Application.h"
#include "Hazard/Modules/ModuleHandler.h"
#include "Hazard/Modules/Renderer/RenderEngine.h"

namespace Hazard {
	void Application::PushModule(Module* module)
	{
		ModuleHandler::PushModule(module);
	}
	void Application::SetTitle(std::string& title)
	{
		ModuleHandler::GetModule<RenderEngine>()->GetWindow().SetWindowTitle(title + " | Hazard");
	}
	Application& Application::GetApplication()
	{
		return *HazardLoop::instance->current;
	}
}
