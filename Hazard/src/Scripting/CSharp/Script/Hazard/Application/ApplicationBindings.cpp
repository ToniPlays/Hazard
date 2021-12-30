#pragma once
#include <hzrpch.h>
#include "ApplicationBindings.h"
#include "Hazard/RenderContext/RenderContext.h"


namespace Hazard::Scripting::CSharp::Bindings {

	BindMap ApplicationBindings::GetBindings()
	{
		BindMap results;
		results.emplace_back("Hazard.Application::Application_Quit_Native", Application::Quit);
		return results;
	}
}