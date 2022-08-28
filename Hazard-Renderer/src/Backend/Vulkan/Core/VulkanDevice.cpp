
#include "VulkanDevice.h"
#ifdef HZR_INCLUDE_VULKAN
#include "../VulkanContext.h"
#include "../VkUtils.h"

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

	VkCommandBuffer VulkanDevice::GetCommandBuffer(bool begin, bool compute) 
	{
		VkCommandBuffer buffer;

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = compute ? m_ComputePool : m_GraphicsPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		VK_CHECK_RESULT(vkAllocateCommandBuffers(m_LogicalDevice, &allocInfo, &buffer), "Failed to allocate command buffer");

		if (begin) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			VK_CHECK_RESULT(vkBeginCommandBuffer(buffer, &beginInfo), "Failed to begin");
		}

		return buffer;
	}
	VkCommandBuffer VulkanDevice::CreateSecondaryCommandBuffer(const std::string& name)
	{
		VkCommandBuffer buffer;

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_GraphicsPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		allocInfo.commandBufferCount = 1;

		VK_CHECK_RESULT(vkAllocateCommandBuffers(m_LogicalDevice, &allocInfo, &buffer), "Failed to allocate command buffer");
		VkUtils::SetDebugUtilsObjectName(m_LogicalDevice, VK_OBJECT_TYPE_COMMAND_BUFFER, name, buffer);

		return buffer;
	}
	void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer)
	{
		FlushCommandBuffer(commandBuffer, m_GraphicsQueue);
	}

	void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue)
	{
		const uint64_t DEFAULT_TIMEOUT = 1000000000;
		HZR_ASSERT(commandBuffer != VK_NULL_HANDLE, "VkCommandBuffer cannot be VK_NULL_HANDLE");
		VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer), "Failed to end VkCommandBuffer");

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = 0;
		
		VkFence fence;
		VK_CHECK_RESULT(vkCreateFence(m_LogicalDevice, &fenceInfo, nullptr, &fence), "Failed to create VkFence");
		VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence), "Failed to submit command buffer");
		VK_CHECK_RESULT(vkWaitForFences(m_LogicalDevice, 1, &fence, VK_TRUE, DEFAULT_TIMEOUT), "VkWaitForFences failed");

		vkDestroyFence(m_LogicalDevice, fence, nullptr);
		vkFreeCommandBuffers(m_LogicalDevice, m_GraphicsPool, 1, &commandBuffer);
	}
}
#endif