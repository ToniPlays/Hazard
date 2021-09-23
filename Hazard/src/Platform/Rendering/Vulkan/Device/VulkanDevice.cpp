#pragma once

#include <hzrpch.h>

#include "VulkanDevice.h"
#include "SwapChain.h"
#include "../Buffers/CommandBuffer.h"

#include "../VKUtils.h"
#include <set>

namespace Hazard::Rendering::Vulkan {

	VulkanDevice::VulkanDevice(Instance* instance)
	{
		m_Surface = &instance->GetSurface();

		m_PhysicalDevice = VKUtils::GetVulkanCapableDevice(instance->GetInstance(), instance->GetSurface());

		if (m_PhysicalDevice == VK_NULL_HANDLE) {
			HZR_THROW("Vulkan physical device not found");
		}

		QueueFamilyIndices indices = VKUtils::GetQueueFamilyIndices(m_PhysicalDevice, instance->GetSurface());
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

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
		poolInfo.flags =
			VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool!");
		}
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * ((int)(sizeof(pool_sizes) / sizeof(*(pool_sizes))));
		pool_info.poolSizeCount = (uint32_t)((int)(sizeof(pool_sizes) / sizeof(*(pool_sizes))));
		pool_info.pPoolSizes = pool_sizes;
		VkResult result = vkCreateDescriptorPool(m_Device, &pool_info, nullptr, &m_DescriptorPool);
		
	}
	VulkanDevice::~VulkanDevice()
	{
		vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
		vkDestroyDevice(m_Device, nullptr);
	}
	void VulkanDevice::SetClearColor(glm::vec4 clearColor)
	{
		m_SwapChain->SetClearColor(clearColor);
	}
	void VulkanDevice::WaitUntilIdle() {

		vkDeviceWaitIdle(m_Device);
	}
	void VulkanDevice::CreateSwapchain(uint32_t imagesInFlight, bool vSync)
	{
		m_SwapChain = std::make_unique<SwapChain>(*this, imagesInFlight, vSync);
	}
	VkCommandBuffer VulkanDevice::BeginSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_Device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}
	void VulkanDevice::EndSingleTimeCommands(VkCommandBuffer buffer)
	{
		vkEndCommandBuffer(buffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &buffer;

		vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_GraphicsQueue);
		vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &buffer);
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
	VkFormat VulkanDevice::GetDepthFormat()
	{
		return VKUtils::FindSupportedFormat(m_PhysicalDevice,
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
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

	}
}