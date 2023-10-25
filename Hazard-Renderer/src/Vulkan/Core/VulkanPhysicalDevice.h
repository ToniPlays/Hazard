#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Core/Device/PhysicalDevice.h"
#include <vector>
#include <unordered_set>
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	struct QueueFamilyIndices 
	{
		int32_t Graphics = -1;
		int32_t Compute = -1;
		int32_t Transfer = -1;
	};


	class VulkanPhysicalDevice : public PhysicalDevice 
	{
		friend class VulkanDevice;
	public:
		
		VulkanPhysicalDevice();
		~VulkanPhysicalDevice();

		std::string GetDeviceName() override { return m_Properties.deviceName; };
		const PhysicalDeviceLimits& GetDeviceLimits() const override { return m_Limits; }
		const PhysicalDeviceCapabilities& GetDeviceCababilities() const { return m_Capabilities; };
		bool SupportsRaytracing() const 
		{ 
			return m_Capabilities.AccelerationStructures && m_Capabilities.RayQuery && m_Capabilities.RayTracingPipeline; 
		};

		//Vulkan specific

		VkPhysicalDevice GetVulkanPhysicalDevice() const { return m_PhysicalDevice; }
		VkFormat GetDepthFormat() const { return m_DepthFormat; }
		QueueFamilyIndices GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }

		static Ref<VulkanPhysicalDevice> Create(int device = -1);

	private:
		void FindCababilities();
		QueueFamilyIndices FindQueueFamilyIndices(int flags);
		VkFormat FindDepthFormat();

	private:
		PhysicalDeviceLimits m_Limits;
		PhysicalDeviceCapabilities m_Capabilities;

		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties m_Properties;
		VkPhysicalDeviceFeatures2 m_Features;
		VkPhysicalDeviceMemoryProperties m_MemoryProperties;

		std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
		QueueFamilyIndices m_QueueFamilyIndices;
		std::vector<VkDeviceQueueCreateInfo> m_QueueCreateInfos;
		
		VkFormat m_DepthFormat;
	};
}
#endif
