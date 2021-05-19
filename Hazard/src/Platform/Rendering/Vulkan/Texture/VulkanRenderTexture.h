#pragma once

#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan {
	class VulkanRenderTexture {
	public:
		VulkanRenderTexture(VkFormat format);
		~VulkanRenderTexture();

		VkRenderPass& GetRenderPass() { return m_RenderPass; }

	private:
		VkRenderPass m_RenderPass;
	};
}