#pragma once
#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan 
{
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	class VKValidationLayer {
	public:
		static bool InitValidationLayers(VkInstanceCreateInfo& info, bool enabled = false);
		static void SetupDebugger(VkInstance instance);
		static bool IsValidationSupported();
	private:
		static VkDebugUtilsMessengerEXT debugMessenger;
		static VkDebugUtilsMessengerCreateInfoEXT GetDebugCreateInfo();
	};
}