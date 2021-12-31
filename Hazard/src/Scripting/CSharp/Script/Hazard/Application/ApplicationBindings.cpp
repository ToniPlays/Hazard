#pragma once
#include <hzrpch.h>
#include "ApplicationBindings.h"
#include "Hazard/RenderContext/RenderContext.h"


namespace Hazard::Scripting::CSharp::Bindings {

	BindMap ApplicationBindings::GetBindings()
	{
		BindMap results;
		results.emplace_back(std::pair<const char*, void*>("Hazard.Application::Application_Quit_Native", (void*) Application::Quit));
		return results;
	}
}