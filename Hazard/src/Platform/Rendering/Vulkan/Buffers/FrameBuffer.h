#pragma once

#include "vulkan/vulkan.h"
#include "../Device/VulkanDevice.h"
#include "RenderPass.h"

namespace Hazard::Rendering::Vulkan
{
	class FrameBuffer {
	public:
		FrameBuffer(VulkanDevice* device, RenderPass* renderPass, std::vector<VkImageView> attachments, VkExtent2D extent);
		~FrameBuffer();

		VkExtent2D GetExtent() { return m_Extent; }
		VkFramebuffer GetBuffer() { return m_FrameBuffer; }

	private:
		VkDevice m_Device;
		VkFramebuffer m_FrameBuffer;
		VkExtent2D m_Extent;
	};
}