#include "VulkanDevice.h"
#include "../VulkanContext.h"

namespace HazardRenderer::Vulkan
{
	VulkanDevice::VulkanDevice(Ref<VulkanPhysicalDevice> physicalDevice, VkPhysicalDeviceFeatures features) : m_PhysicalDevice(physicalDevice), m_EnabledFeatures(features)
	{
		const bool enableAftermath = false;
		
		std::vector<const char*> additionalExtensions;
		HZR_ASSERT(m_PhysicalDevice->IsExtensionSupported(VK_KHR_SWAPCHAIN_EXTENSION_NAME), "Swapchain extension not supported");
		additionalExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		if (m_PhysicalDevice->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME))
			additionalExtensions.push_back(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);
		if (m_PhysicalDevice->IsExtensionSupported(VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME))
			additionalExtensions.push_back(VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME);

		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.queueCreateInfoCount = (uint32_t)m_PhysicalDevice->m_QueueCreateInfos.size();
		deviceInfo.pQueueCreateInfos = m_PhysicalDevice->m_QueueCreateInfos.data();
		deviceInfo.pEnabledFeatures = &features;

		VkPhysicalDeviceFeatures2 physicalFeatures2 = {};
		if (m_PhysicalDevice->IsExtensionSupported(VK_EXT_DEBUG_MARKER_EXTENSION_NAME)) 
		{
			additionalExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
			m_EnableDebugMarkers = true;
		}

		if (additionalExtensions.size() > 0)
		{
			deviceInfo.enabledExtensionCount = (uint32_t)additionalExtensions.size();
			deviceInfo.ppEnabledExtensionNames = additionalExtensions.data();
		}

		VkResult result = vkCreateDevice(m_PhysicalDevice->GetVulkanPhysicalDevice(), &deviceInfo, nullptr, &m_LogicalDevice);
		HZR_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan Logical device");

		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = m_PhysicalDevice->m_QueueFamilyIndices.Graphics;
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		VK_CHECK_RESULT(vkCreateCommandPool(m_LogicalDevice, &cmdPoolInfo, nullptr, &m_GraphicsPool), "Failed to create Vulkan Graphics Command Pool");

		cmdPoolInfo.queueFamilyIndex = m_PhysicalDevice->m_QueueFamilyIndices.Compute;
		VK_CHECK_RESULT(vkCreateCommandPool(m_LogicalDevice, &cmdPoolInfo, nullptr, &m_ComputePool), "Failed to create Vulkan Compute Command Pool");

		vkGetDeviceQueue(m_LogicalDevice, m_PhysicalDevice->m_QueueFamilyIndices.Graphics, 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_LogicalDevice, m_PhysicalDevice->m_QueueFamilyIndices.Compute, 0, &m_ComputeQueue);
	}
	void VulkanDevice::Destroy()
	{
		vkDestroyCommandPool(m_LogicalDevice, m_GraphicsPool, nullptr);
		vkDestroyCommandPool(m_LogicalDevice, m_ComputePool, nullptr);

		vkDeviceWaitIdle(m_LogicalDevice);
		vkDestroyDevice(m_LogicalDevice, nullptr);
	}
}
