#pragma once

#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan {

	class VKValidationLayer {
	public:

		static bool InitValidationLayers(bool enable, VkInstanceCreateInfo& info);
		static bool IsValidationLayersSupported();
	};
}