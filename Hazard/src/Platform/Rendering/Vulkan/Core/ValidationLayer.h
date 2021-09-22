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
		static void SetupDebugger(VkInstance instance);
		static bool IsValidationSupported();
		static void GetDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	private:
		static VkDebugUtilsMessengerEXT debugMessenger;
	};
}