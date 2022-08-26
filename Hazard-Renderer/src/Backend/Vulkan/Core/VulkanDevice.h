#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/PhysicalDevice.h"
#include <vector>
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	struct QueueFamilyIndices {
		int32_t Graphics = -1;
		int32_t Compute = -1;
		int32_t Transfer = -1;
	};


	class VulkanPhysicalDevice : public PhysicalDevice {
	public:
		
		VulkanPhysicalDevice();
		~VulkanPhysicalDevice();

		std::string GetDeviceName() override { return m_Properties.deviceName; };
		const PhysicalDeviceLimits& GetDeviceLimits() const override { return m_Limits; }

		static Ref<VulkanPhysicalDevice> Create(int device = -1);

	private:

		QueueFamilyIndices GetQueueFamilyIndices(int flags);
		VkFormat FindDepthFormat();

	private:

		PhysicalDeviceLimits m_Limits;

		VkPhysicalDevice m_PhysicalDevice;
		VkPhysicalDeviceProperties m_Properties;
		VkPhysicalDeviceFeatures m_Features;
		VkPhysicalDeviceMemoryProperties m_MemoryProperties;

		std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
		QueueFamilyIndices m_QueueFamilyIndices;
		std::vector<VkDeviceQueueCreateInfo> m_QueueCreateInfos;
		
		VkFormat m_DepthFormat;
	};
}
#endif
