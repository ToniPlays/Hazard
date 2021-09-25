#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Hazard::Rendering::Vulkan 
{
	class WindowSurface {
	public:
		WindowSurface(VkInstance instance, GLFWwindow* window);
		~WindowSurface();

		VkSurfaceKHR GetVkSurface() { return m_Surface; }
		operator VkSurfaceKHR () const { return m_Surface; }

	private:
		VkInstance m_Instance;
		VkSurfaceKHR m_Surface;
	};
}