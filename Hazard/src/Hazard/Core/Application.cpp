#pragma once

#include <hzrpch.h>
#include "Application.h"

namespace Hazard {

	void Application::SetTitle(std::string title)
	{
		SetTitle(title.c_str());
	}
	void Application::SetTitle(const char* title)
	{
		std::cout << "Application::SetTitle(const char* title) not defined" << std::endl;
	}
}
