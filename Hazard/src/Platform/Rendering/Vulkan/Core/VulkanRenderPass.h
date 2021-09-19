#pragma once

#include "VulkanDevice.h"
#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan 
{

	class VulkanSwapChain;

	class VulkanRenderPass {
	public:
		VulkanRenderPass(VulkanDevice* device, VulkanSwapChain* swapChain);
		~VulkanRenderPass();

		VkRenderPass& GetRenderPass() { return m_RenderPass; }

	private:
		VkRenderPass m_RenderPass;
	};
}
