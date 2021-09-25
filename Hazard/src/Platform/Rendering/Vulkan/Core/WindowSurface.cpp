#pragma once

#include <hzrpch.h>
#include "WindowSurface.h"

namespace Hazard::Rendering::Vulkan {

	WindowSurface::WindowSurface(VkInstance instance, GLFWwindow* window)
	{
		VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &m_Surface);
		HZR_CORE_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan window surface");
		m_Instance = instance;
	}
	WindowSurface::~WindowSurface()
	{
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
	}
}