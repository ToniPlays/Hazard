#pragma once

#include <vulkan/vulkan.h>
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"

namespace Hazard::Rendering::Vulkan {
	class VulkanDescriptorPool {
	public:
		VulkanDescriptorPool(VulkanDevice* device, VulkanSwapChain* swapChain);
		~VulkanDescriptorPool();

		VkDescriptorPool GetPool() { return m_DescriptorPool; }

	private:
		VkDescriptorPool m_DescriptorPool;
	};
}
