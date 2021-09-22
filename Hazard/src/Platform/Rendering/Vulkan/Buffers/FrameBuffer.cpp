#pragma once
#include <hzrpch.h>
#include "FrameBuffer.h"

namespace Hazard::Rendering::Vulkan 
{
	FrameBuffer::FrameBuffer(VulkanDevice* device, RenderPass* renderPass, std::vector<VkImageView> attachments, VkExtent2D extent)
	{
		m_Extent = extent;
		m_Device = device->GetDevice();

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass->GetRenderPass();
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = extent.width;
		framebufferInfo.height = extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device->GetDevice(), &framebufferInfo, nullptr,	&m_FrameBuffer) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan FrameBuffer!");
		}
	}
	FrameBuffer::~FrameBuffer()
	{
		vkDestroyFramebuffer(m_Device, m_FrameBuffer, nullptr);
	}
}
