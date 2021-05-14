#pragma once

#include <hzrpch.h>
#include "VKValidationLayers.h"
#include "VKUtils.h"

namespace Hazard::Rendering::Vulkan 
{
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};




	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cout << "VULKAN: \n" << "Validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	bool VKValidationLayer::InitValidationLayers(bool enable, VkInstanceCreateInfo& info)
	{
		if (!enable) {
			info.enabledLayerCount = 0;
			info.pNext = NULL;
			return true;
		}
		
		info.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		info.ppEnabledLayerNames = validationLayers.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
										  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
										  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT         | 
									  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT      | 
									  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		debugCreateInfo.pfnUserCallback = debugCallback;
		debugCreateInfo.pNext = NULL;
		
		info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

		HZR_CORE_INFO("Vulkan validation enabled");

		return true;
	}
	bool VKValidationLayer::IsValidationLayersSupported()
	{
		uint32_t layerCount = 0;

		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> layers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		for (const char* name : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProps : layers) {
				if (strcmp(name, layerProps.layerName) == 0) {
					layerFound = true;
					break;
				}
			}
			if (!layerFound) return false;
		}

		return true;
	}
}