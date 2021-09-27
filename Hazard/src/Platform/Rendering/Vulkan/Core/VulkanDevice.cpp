#pragma once

#include <hzrpch.h>

#include "VulkanDevice.h"
#include "../VulkanContext.h"

#include "../VKUtils.h"
#include <set>

namespace Hazard::Rendering::Vulkan {

	VulkanDevice::VulkanDevice(VkInstance instance, VkSurfaceKHR surface)
	{
		m_PhysicalDevice = VKUtils::GetVulkanCapableDevice(instance, surface);

		if (m_PhysicalDevice == VK_NULL_HANDLE) {
			HZR_THROW("Vulkan physical device not found");
		}

		QueueFamilyIndices indices = VKUtils::GetQueueFamilyIndices(m_PhysicalDevice, surface);
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}
		VkPhysicalDeviceFeatures deviceFeatures = {};
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan physical device!");
		}
		vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);

		CreateCommandPool();
	}
	VulkanDevice::~VulkanDevice()
	{
		vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
		vkDestroyDevice(m_Device, nullptr);
	}
	void VulkanDevice::WaitUntilIdle() {

		vkDeviceWaitIdle(m_Device);
	}
	DeviceSpec VulkanDevice::GetSpec()
	{
		DeviceSpec spec;
		VkPhysicalDeviceProperties props = {};
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &props);
		std::stringstream ss;

		ss << "Vulkan " << VK_VERSION_MAJOR(props.apiVersion);
		ss << "." << VK_VERSION_MINOR(props.apiVersion);
		ss << "." << VK_VERSION_PATCH(props.apiVersion);

		spec.renderer = ss.str();
		spec.name = props.deviceName;
		spec.textureSlots = props.limits.framebufferNoAttachmentsSampleCounts + 1;
		return spec;
	}
	uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) &&
				(memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}
		return 0;
	}
	void VulkanDevice::CreateCommandPool()
	{
		QueueFamilyIndices indices = VKUtils::GetQueueFamilyIndices(m_PhysicalDevice, VulkanContext::GetSurface());

		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = indices.graphicsFamily.value();
		
		auto result = vkCreateCommandPool(m_Device, &createInfo, nullptr, &m_CommandPool);
		HZR_CORE_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan CommandPool");
	}
}