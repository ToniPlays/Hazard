
#include "VulkanDevice.h"
#ifdef HZR_INCLUDE_VULKAN
#include "../VulkanContext.h"

#include <set>

namespace HazardRenderer::Vulkan {

	
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
			if (m_Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				selectedDevice = device;
				break;
			}
		}
		
		HZR_ASSERT(selectedDevice, "Failed to get discrete Vulkan GPU");
		m_PhysicalDevice = selectedDevice;

		vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_Features);
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &m_MemoryProperties);

		//Queue family properties
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
		m_QueueFamilyProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, m_QueueFamilyProperties.data());

		//Device extension properties
		uint32_t extCount = 0;
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extCount, nullptr);
		if (extCount > 0) 
		{
			std::vector<VkExtensionProperties> extensions(extCount);
			if (vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extCount, &extensions.front()) == VK_SUCCESS) {
				std::cout << "Device extensions: " << std::endl;
				for (const auto& ext : extensions) {
					std::cout << " -" << ext.extensionName << std::endl;
				}
			}
		}

		//Queue families
		static const float defaultPriority = 0.0f;
		
		int requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
		m_QueueFamilyIndices = GetQueueFamilyIndices(requestedQueueTypes);


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
				queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Compute;
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

	QueueFamilyIndices VulkanPhysicalDevice::GetQueueFamilyIndices(int flags)
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
