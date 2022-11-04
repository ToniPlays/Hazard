
#include "VulkanPhysicalDevice.h"
#ifdef HZR_INCLUDE_VULKAN
#include "../VulkanContext.h"
#include "../VkUtils.h"

#include <set>

namespace HazardRenderer::Vulkan 
{
	VulkanPhysicalDevice::VulkanPhysicalDevice()
	{
		VkInstance instance = VulkanContext::GetVulkanInstance();

		uint32_t deviceCount = 0;
		VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr), "Failed to enumerate physical devices");
		HZR_ASSERT(deviceCount > 0, "No available Vulkan capable devices");
		std::vector<VkPhysicalDevice> devices(deviceCount);
		VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()), "Failed to enumerate physical devices");

		VkPhysicalDevice selectedDevice;
		for (VkPhysicalDevice& device : devices)
		{
			vkGetPhysicalDeviceProperties(device, &m_Properties);
			//TODO: select others probably?
			if (m_Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) 
			{
				m_Capabilities.Discrete = true;
				selectedDevice = device;
				break;
			}
		}

		HZR_ASSERT(selectedDevice, "Failed to get discrete Vulkan GPU");
		m_PhysicalDevice = selectedDevice;

		FindCababilities();

		m_Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		m_Features.pNext = nullptr;

		vkGetPhysicalDeviceFeatures2(m_PhysicalDevice, &m_Features);
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &m_MemoryProperties);

		//Queue family properties
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
		m_QueueFamilyProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, m_QueueFamilyProperties.data());

		//Queue families
		static const float defaultPriority = 0.0f;
		
		int requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
		m_QueueFamilyIndices = FindQueueFamilyIndices(requestedQueueTypes);


		if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT) 
		{
			VkDeviceQueueCreateInfo queueInfo = {};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Graphics;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultPriority;
			m_QueueCreateInfos.push_back(queueInfo);
		}
		if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
		{
			VkDeviceQueueCreateInfo queueInfo = {};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Compute;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultPriority;
			m_QueueCreateInfos.push_back(queueInfo);
		}
		if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
		{
			if ((m_QueueFamilyIndices.Transfer != m_QueueFamilyIndices.Graphics) && (m_QueueFamilyIndices.Transfer != m_QueueFamilyIndices.Compute)) {
				VkDeviceQueueCreateInfo queueInfo = {};
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Transfer;
				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &defaultPriority;
				m_QueueCreateInfos.push_back(queueInfo);
			}
		}
		
		m_DepthFormat = FindDepthFormat();
	}
	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{

	}
	Ref<VulkanPhysicalDevice> VulkanPhysicalDevice::Create(int device)
	{
		return Ref<VulkanPhysicalDevice>::Create();
	}

	void VulkanPhysicalDevice::FindCababilities()
	{
		uint32_t extCount = 0;
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extCount, nullptr);
		if (extCount > 0)
		{
			std::vector<VkExtensionProperties> extensions(extCount);
			if (vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
			{
				for (const auto& ext : extensions) 
				{
					std::cout << " -" << ext.extensionName << std::endl;
					m_Capabilities.Swapchain				|= strcmp(ext.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0;
					m_Capabilities.DiagnosticCheckpoint		|= strcmp(ext.extensionName, VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME) == 0;
					m_Capabilities.DiagnosticConfig			|= strcmp(ext.extensionName, VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME) == 0;
					m_Capabilities.DebugMarker				|= strcmp(ext.extensionName, VK_EXT_DEBUG_MARKER_EXTENSION_NAME) == 0;

					m_Capabilities.AccelerationStructures	|= strcmp(ext.extensionName, VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME) == 0;
					m_Capabilities.RayTracingPipeline		|= strcmp(ext.extensionName, VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME) == 0;
					m_Capabilities.RayQuery					|= strcmp(ext.extensionName, VK_KHR_RAY_QUERY_EXTENSION_NAME) == 0;
				}
			}
		}

		m_Limits.MaxTextureUnits = m_Properties.limits.maxPerStageDescriptorSampledImages;
		m_Limits.MaxFrameBufferSize.x = m_Properties.limits.maxFramebufferWidth;
		m_Limits.MaxFrameBufferSize.y = m_Properties.limits.maxFramebufferHeight;

		if (SupportsRaytracing())
		{
			VkPhysicalDeviceRayTracingPropertiesNV rtProperties = {};
			rtProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV;

			VkPhysicalDeviceProperties2 properties = {};
			properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
			properties.pNext = &rtProperties;

			vkGetPhysicalDeviceProperties2(m_PhysicalDevice, &properties);

			m_Limits.MaxRecursionDepth = rtProperties.maxRecursionDepth;
		}
	}

	QueueFamilyIndices VulkanPhysicalDevice::FindQueueFamilyIndices(int flags)
	{
		QueueFamilyIndices result;

		if (flags & VK_QUEUE_COMPUTE_BIT) 
		{
			for (uint32_t i = 0; i < m_QueueFamilyProperties.size(); i++) {
				const auto& prop = m_QueueFamilyProperties[i];
				if ((prop.queueFlags & VK_QUEUE_COMPUTE_BIT) && ((prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
					result.Compute = i;
					break;
				}
			}
		}
		if (flags & VK_QUEUE_TRANSFER_BIT)
		{
			for (uint32_t i = 0; i < m_QueueFamilyProperties.size(); i++) {
				const auto& prop = m_QueueFamilyProperties[i];
				if ((prop.queueFlags & VK_QUEUE_TRANSFER_BIT) && ((prop.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)) {
					result.Transfer = i;
					break;
				}
			}
		}

		for (uint32_t i = 0; i < m_QueueFamilyProperties.size(); i++) {
			if ((flags & VK_QUEUE_TRANSFER_BIT) && result.Transfer == -1) {
				result.Transfer = i;
			}
			if ((flags & VK_QUEUE_COMPUTE_BIT) && result.Compute == -1) {
				result.Compute = i;
			}
			if ((flags & VK_QUEUE_GRAPHICS_BIT) && result.Graphics == -1) {
				result.Graphics = i;
			}
		}

		return result;
	}
	VkFormat VulkanPhysicalDevice::FindDepthFormat()
	{
		std::vector<VkFormat> formats = {
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM
		};
		for (auto& format : formats) {
			VkFormatProperties formatProps;
			vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &formatProps);
			if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
				return format;
		}
		return VK_FORMAT_UNDEFINED;
	}
}
#endif
