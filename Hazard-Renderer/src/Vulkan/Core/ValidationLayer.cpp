
#include "ValidationLayer.h"
#ifdef HZR_INCLUDE_VULKAN

#include "../VKUtils.h"
#include "../VulkanContext.h"
#include "Core/Window.h"

#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan {

	VkDebugUtilsMessengerEXT ValidationLayer::debugMessenger;

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) 
	{
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) 
			return VK_FALSE;
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) 
			return VK_FALSE;

		Window::SendDebugMessage({ Severity::Trace, pCallbackData->pMessage });
		return VK_FALSE;
	}

	bool ValidationLayer::InitValidationLayers(VkInstanceCreateInfo& info, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo, bool enabled)
	{
		if (!enabled) 
		{
			info.enabledLayerCount = 0;
			info.pNext = NULL;
			return true;
		}

		GetDebugCreateInfo(debugCreateInfo);

		info.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		info.ppEnabledLayerNames = validationLayers.data();
		info.pNext = &debugCreateInfo;

		return true;
	}
	bool ValidationLayer::SetupDebugger(VkInstance instance)
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		GetDebugCreateInfo(createInfo);

		Window::SendDebugMessage({ Severity::Info, "Vulkan debugger enabled" });

		VkResult result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
		return result == VK_SUCCESS;
	}
	bool ValidationLayer::IsValidationSupported()
	{
		uint32_t layerCount = 0;

		VK_CHECK_RESULT(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));
		std::vector<VkLayerProperties> layers(layerCount);

		for (const char* name : validationLayers) {
			bool layerFound = false;
			for (const auto& layerProps : layers) {
				if (strcmp(name, layerProps.layerName) == 0) {
					layerFound = true;
					break;
				}
			}
			if (!layerFound) 
				return false;
		}
		return true;
	}
	void ValidationLayer::GetDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pNext = NULL;
	}
}
#endif
