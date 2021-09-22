#pragma once

#include <hzrpch.h>
#include "WindowSurface.h"
#include "../Core/Instance.h"

namespace Hazard::Rendering::Vulkan {

	WindowSurface::WindowSurface(Instance* instance, GLFWwindow* window)
	{
		VkResult result = glfwCreateWindowSurface(instance->GetInstance(), window, nullptr, &m_Surface);
		if (result != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan window surface");
		}
		m_Instance = instance->GetInstance();
		HZR_CORE_INFO("Created window surface for Vulkan");
	}
	WindowSurface::~WindowSurface()
	{
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
	}
}