#pragma once

#include <hzrpch.h>
#include "VulkanWindowSurface.h"
#include <GLFW/glfw3.h>
#include "../VKContext.h"

namespace Hazard::Rendering::Vulkan {

	VulkanWindowSurface::VulkanWindowSurface()
	{
		if (glfwCreateWindowSurface(VulkanInstance::GetData().Instance, VulkanInstance::GetData().window, nullptr, &m_Surface) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan window surface");
		}
		HZR_CORE_INFO("Created window surface for Vulkan");
	}
	VulkanWindowSurface::~VulkanWindowSurface()
	{
		vkDestroySurfaceKHR(VulkanInstance::GetData().Instance, m_Surface, nullptr);
	}
}