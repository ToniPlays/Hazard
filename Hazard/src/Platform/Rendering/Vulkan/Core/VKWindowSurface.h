#pragma once

#include <vulkan/vulkan.h>


namespace Hazard::Rendering::Vulkan {
	class VKWindowSurface {
	public:
		VKWindowSurface();
		~VKWindowSurface();

		VkSurfaceKHR GetSurface() { return surface; }

	private:
		VkSurfaceKHR surface;
	};
}