#pragma once

#include <hzrpch.h>
#include "VulkanRenderpass.h"
#include "VulkanSwapChain.h"

namespace Hazard::Rendering::Vulkan 
{
	VulkanRenderPass::VulkanRenderPass(VulkanDevice* device, VulkanSwapChain* swapChain)
	{
		VkAttachmentDescription colorAttachment {};
		colorAttachment.format = swapChain->GetImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorRef {};
		colorRef.attachment = 0;
		colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorRef;

		VkRenderPassCreateInfo createInfo {};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &colorAttachment;
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpass;

		if (vkCreateRenderPass(device->GetDevice(), &createInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan RenderPass");
		}
	}
	VulkanRenderPass::~VulkanRenderPass()
	{

	}
}