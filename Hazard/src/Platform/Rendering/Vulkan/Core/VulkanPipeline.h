#pragma once

#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan {
	class VulkanPipeline {
	public:
		VulkanPipeline(VkRenderPass renderPass, VkExtent2D swapChainExtent);
		~VulkanPipeline();

		VkPipeline GetPipeline() { return m_Pipeline; }
		VkPipelineCache GetPipelineCache() { return m_Cache; }

	private:
		VkPipelineLayout m_Layout;
		VkPipeline m_Pipeline;
		VkPipelineCache m_Cache;
	};
}