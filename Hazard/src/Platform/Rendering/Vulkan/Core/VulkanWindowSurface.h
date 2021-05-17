#pragma once

#include <vulkan/vulkan.h>


namespace Hazard::Rendering::Vulkan {
	class VulkanWindowSurface {
	public:
		VulkanWindowSurface();
		~VulkanWindowSurface();

		VkSurfaceKHR GetSurface() { return m_Surface; }

	private:
		VkSurfaceKHR m_Surface;
	};
}