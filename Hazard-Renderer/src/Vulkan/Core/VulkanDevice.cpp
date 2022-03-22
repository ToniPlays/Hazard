
#include "VulkanDevice.h"
#include "../VulkanContext.h"

#include "../VKUtils.h"
#include <set>

namespace HazardRenderer::Vulkan {

	
	VulkanDevice::VulkanDevice(VkInstance instance, VkSurfaceKHR surface, uint32_t imagesInFlight)
	{
		m_PhysicalDevice = VKUtils::GetVulkanCapableDevice(instance, surface);
		ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "Failed to get capable device");

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
		deviceFeatures.fillModeNonSolid = VK_TRUE;
		deviceFeatures.wideLines = VK_TRUE;

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();


		if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS) {
			THROW_EXCEPT("Failed to create Vulkan physical device!");
		}

		m_GraphicsQueue.Index = indices.graphicsFamily.value();
		m_PresentQueue.Index = indices.presentFamily.value();
		vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue.Queue);
		vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue.Queue);


		VkPipelineCacheCreateInfo cache = {};
		cache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		VK_CHECK_RESULT(vkCreatePipelineCache(m_Device, &cache, nullptr, &m_PipelineCache));

		//Since all depth formats may be optional, we need to find a suitable depth format to use
		//Start with the highest precision packed format
		std::vector<VkFormat> depthFormats = {
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM
		};

		//TODO: Move to VulkanPhysicalDevice
		for (auto& format : depthFormats)
		{
			VkFormatProperties formatProps;
			vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &formatProps);
			//Format must support depth stencil attachment for optimal tiling
			if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
				m_DepthFormat = format;
				break;
			}
		}

		VkPhysicalDeviceProperties props = {};
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &props);
		m_DeviceName = props.deviceName;

		//CreatePools(imagesInFlight);	}
	VulkanDevice::~VulkanDevice()
	{
		vkDestroyPipelineCache(m_Device, m_PipelineCache, nullptr);
		//for(auto pool : m_DescriptorPools)
		//	vkDestroyDescriptorPool(m_Device, pool, nullptr);

		//vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
		//vkDestroyPipelineCache(m_Device, m_PipelineCache, nullptr);
		//vkDestroyDevice(m_Device, nullptr);
	}
	
	void VulkanDevice::WaitUntilIdle() {

		VK_CHECK_RESULT(vkDeviceWaitIdle(m_Device));
	}
	/*
	VkCommandBuffer VulkanDevice::GetCommandBuffer(bool begin)
	{
		VkCommandBuffer cmdBuffer;

		VkCommandBufferAllocateInfo cmdAllocInfo = {};
		cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdAllocInfo.commandPool = m_CommandPool;
		cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdAllocInfo.commandBufferCount = 1;

		VK_CHECK_RESULT(vkAllocateCommandBuffers(m_Device, &cmdAllocInfo, &cmdBuffer));
		
		if (begin) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			VK_CHECK_RESULT(vkBeginCommandBuffer(cmdBuffer, &beginInfo));
		}

		return cmdBuffer;
	}

	VkCommandBuffer VulkanDevice::CreateSecondaryCommandBuffer()
	{
		VkCommandBuffer cmdBuffer;

		VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
		cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdBufAllocateInfo.commandPool = m_CommandPool;
		cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		cmdBufAllocateInfo.commandBufferCount = 1;

		VK_CHECK_RESULT(vkAllocateCommandBuffers(m_Device, &cmdBufAllocateInfo, &cmdBuffer));
		return cmdBuffer;
	}

	void VulkanDevice::FlushCommandBuffer(VkCommandBuffer buffer)
	{
		FlushGraphicsCommandBuffer(buffer);
	}
	void VulkanDevice::FlushGraphicsCommandBuffer(VkCommandBuffer buffer)
	{
		const uint64_t TIMEOUT = 100000000000;

		VK_CHECK_RESULT(vkEndCommandBuffer(buffer));

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &buffer;

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = 0;

		VkFence fence;
		VK_CHECK_RESULT(vkCreateFence(m_Device, &fenceCreateInfo, nullptr, &fence));
		VK_CHECK_RESULT(vkQueueSubmit(m_GraphicsQueue.Queue, 1, &submitInfo, fence));

		VK_CHECK_RESULT(vkWaitForFences(m_Device, 1, &fence, VK_TRUE, TIMEOUT));
		vkDestroyFence(m_Device, fence, nullptr);
		vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &buffer);
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
	void VulkanDevice::CreatePools(uint32_t count)
	{
		/*QueueFamilyIndices indices = VKUtils::GetQueueFamilyIndices(m_PhysicalDevice, VulkanContext::GetSurface());

		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = indices.graphicsFamily.value();
		
		auto result = vkCreateCommandPool(m_Device, &createInfo, nullptr, &m_CommandPool);
		ASSERT(result == VK_SUCCESS, "Failed to create Vulkan CommandPool");

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

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 11;
		poolInfo.pPoolSizes = pool_sizes;
		poolInfo.maxSets = 100000;

		m_DescriptorPools.resize(count);
		for (size_t i = 0; i < count; i++) {
			VK_CHECK_RESULT(vkCreateDescriptorPool(m_Device, &poolInfo, nullptr, &m_DescriptorPools[i]));
		}
	}*/
}