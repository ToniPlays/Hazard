#pragma once

#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan 
{
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	class ValidationLayer {
	public:
		static bool InitValidationLayers(VkInstanceCreateInfo& info, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo, bool enabled = false);
		static bool SetupDebugger(VkInstance instance);
		static bool IsValidationSupported();
	private:
		static void GetDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		static VkDebugUtilsMessengerEXT debugMessenger;
	};
}