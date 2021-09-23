#pragma once

#include <hzrpch.h>
#include "RenderPass.h"

#include "../VKUtils.h"
#include "../Device/SwapChain.h"
#include "../Buffers/CommandBuffer.h"
#include "VulkanImage.h"

namespace Hazard::Rendering::Vulkan
{
	RenderPass::RenderPass(VulkanDevice* device, VkFormat swapChainFormat)
	{
		m_Device = device->GetDevice();

		VkAttachmentDescription depthAttachment = {};
		depthAttachment.format = device->GetDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = swapChainFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask =
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstSubpass = 0;
		dependency.dstStageMask =
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask =
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(device->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan RenderPass!");
		}
	}
	RenderPass::~RenderPass()
	{
		vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
	}
	void RenderPass::Begin(CommandBuffer* buffer, VkFramebuffer frameBuffer, VkExtent2D extent)
	{
		VkRenderPassBeginInfo passInfo = {};
		passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		passInfo.renderPass = m_RenderPass;
		passInfo.framebuffer = frameBuffer;
		passInfo.renderArea.offset = { 0, 0 };
		passInfo.renderArea.extent = extent;

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.02f, 0.02f, 0.02f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		passInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		passInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(*buffer->GetBuffer(), &passInfo, VK_SUBPASS_CONTENTS_INLINE);
	}
	void RenderPass::End(CommandBuffer* buffer)
	{
		vkCmdEndRenderPass(*buffer->GetBuffer());
	}
	void RenderPass::CreateDepthImages(VulkanDevice* device, VkExtent2D extent, uint32_t count)
	{
		VkFormat depthFormat = device->GetDepthFormat();
		m_DepthImages.resize(count);
		m_DepthMemorys.resize(count);
		m_DepthImageViews.resize(count);
		
		for (size_t i = 0; i < count; i++) {
			m_DepthImages[i] = VulkanImage::CreateImage(m_DepthMemorys[i], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TYPE_2D,  extent, depthFormat);
			m_DepthImageViews[i] = VulkanImage::CreateImageView(m_DepthImages[i], VK_IMAGE_VIEW_TYPE_2D, depthFormat,
				VK_IMAGE_ASPECT_DEPTH_BIT);
		}
	}
}