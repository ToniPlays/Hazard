#pragma once

#include "Ref.h"

#include "VulkanPhysicalDevice.h"

namespace HazardRenderer::Vulkan 
{
	class VulkanDevice : public RefCount {
	public:
		VulkanDevice(Ref<VulkanPhysicalDevice> physicalDevice, VkPhysicalDeviceFeatures features);
		~VulkanDevice() = default;

		void Destroy();

		Ref<PhysicalDevice> GetPhysicalDevice() { return m_PhysicalDevice; }

		VkDevice GetVulkanDevice() const { return m_LogicalDevice; }
		VkCommandPool GetGraphicsPool() const { return m_GraphicsPool; }
		VkCommandPool GetComputePool() const { return m_ComputePool; }

		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		VkQueue GetComputesQueue() const { return m_ComputeQueue; }

	private:
		Ref<VulkanPhysicalDevice> m_PhysicalDevice;
		VkPhysicalDeviceFeatures m_EnabledFeatures;

		VkDevice m_LogicalDevice;
		VkCommandPool m_GraphicsPool;
		VkCommandPool m_ComputePool;

		VkQueue m_GraphicsQueue;
		VkQueue m_ComputeQueue;

		bool m_EnableDebugMarkers = false;
	};
}