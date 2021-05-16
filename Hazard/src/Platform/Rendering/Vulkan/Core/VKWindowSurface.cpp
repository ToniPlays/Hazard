#pragma once

#include <hzrpch.h>
#include "VKWindowSurface.h"
#include <GLFW/glfw3.h>
#include "../VKContext.h"

namespace Hazard::Rendering::Vulkan {

	VKWindowSurface::VKWindowSurface()
	{
		if (glfwCreateWindowSurface(VKInstance::GetData().Instance, VKInstance::GetData().window, nullptr, &surface) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan window surface");
		}
		HZR_CORE_INFO("Created window surface for Vulkan");
	}
	VKWindowSurface::~VKWindowSurface()
	{

	}
}