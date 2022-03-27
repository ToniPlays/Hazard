

#include "WindowSurface.h"
#ifdef HZR_INCLUDE_VULKAN

namespace HazardRenderer::Vulkan {

	WindowSurface::WindowSurface(VkInstance instance, GLFWwindow* window)
	{
		VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &m_Surface);
		ASSERT(result == VK_SUCCESS, "Failed to create Vulkan window surface");
		m_Instance = instance;
	}
	WindowSurface::~WindowSurface()
	{
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
	}
}
#endif
