#pragma once
#include <hzrpch.h>
#include "ApplicationBindings.h"

namespace Hazard::Scripting::Bindings {

	BindMap ApplicationBindings::GetBindings()
	{
		BindMap results;
		results.emplace_back("Hazard.Application::Application_Quit_Native", Application::Quit);
		return results;
	}
}
