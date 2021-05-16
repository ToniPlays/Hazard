#pragma once

#include <hzrpch.h>
#include "VKDevice.h"
#include "VKInstance.h"

#include "../VKUtils.h"
#include <set>

namespace Hazard::Rendering::Vulkan {

	VKDevice::VKDevice()
	{
		m_PhysicalDevice = VKUtils::GetVulkanCapableDevice(VKInstance::GetInstance());

		if (m_PhysicalDevice == VK_NULL_HANDLE) {
			HZR_THROW("Vulkan physical device not found");
		}

		QueueFamilyIndices indices = VKUtils::GetQueueFamilyIndices(m_PhysicalDevice, VKInstance::GetData().Surface);
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

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
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
		vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_GraphicsQueue);

		HZR_CORE_INFO("Created Vulkan Queue for device");
	}
	VKDevice::~VKDevice()
	{
		vkDestroyDevice(m_Device, nullptr);
	}
}