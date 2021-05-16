#pragma once

#include <hzrpch.h>
#include "VKValidationLayer.h"
#include "../VKUtils.h"
#include "../VKContext.h"
#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan {

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

		VKContext::SendDebugMessage(pCallbackData->pMessage, "Vulkan");

		return VK_FALSE;
	}

	VkDebugUtilsMessengerEXT VKValidationLayer::debugMessenger;

	bool VKValidationLayer::InitValidationLayers(VkInstanceCreateInfo& info, bool enabled)
	{
		if (!enabled) {
			info.enabledLayerCount = 0;
			info.pNext = NULL;
			return true;
		}

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		VKValidationLayer::GetDebugCreateInfo(debugCreateInfo);

		info.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		info.ppEnabledLayerNames = validationLayers.data();
		info.pNext = &debugCreateInfo;

		HZR_CORE_INFO("Vulkan validation enabled");
		return true;
	}
	void VKValidationLayer::SetupDebugger(VkInstance instance)
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		GetDebugCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			HZR_THROW("Failed to create Debug Messenger!");
		}
	}
	bool VKValidationLayer::IsValidationSupported()
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
			if (!layerFound) 
				return false;
		}

		return true;
	}
	void VKValidationLayer::GetDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pNext = NULL;
	}
}