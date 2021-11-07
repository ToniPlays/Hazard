#pragma once
#include <hzrpch.h>
#include "VulkanFrameBuffer.h"
#include "VulkanContext.h"
#include "VKUtils.h"

namespace Hazard::Rendering::Vulkan
{
	VulkanFrameBuffer::VulkanFrameBuffer(FrameBufferCreateInfo* info)
	{
		m_Specs.DebugName = info->DebugName;

		m_Specs.SwapChainTarget = info->SwapChainTarget;
		m_Specs.ClearOnLoad = info->ClearOnLoad;
		m_Specs.Samples = info->Samples;
		m_Specs.pFrameBuffer = info->pFrameBuffer;
		m_Specs.AttachmentCount = info->AttachmentCount;
		m_Specs.Attachments = info->Attachments;
		m_Specs.ClearColor = info->ClearColor;

		if (m_Specs.Width == 0) {
			m_Specs.Width = VulkanContext::GetSwapchain()->GetWidth();
			m_Specs.Height = VulkanContext::GetSwapchain()->GetHeight();
		}
		else
		{
			m_Specs.Width = info->Width;
			m_Specs.Height = info->Height;
		}
		uint32_t attachmentIndex = 0;
		if (!info->pFrameBuffer) {
			for (auto& attachment : m_Specs.Attachments) {
				if (VKUtils::IsDepth(attachment.Format))
				{
					Image2DCreateInfo imageInfo = {};
					imageInfo.Width = m_Specs.Width;
					imageInfo.Height = m_Specs.Height;
					imageInfo.Usage = ImageUsage::Attachment;
					imageInfo.Format = attachment.Format;

					m_DepthAttachment = Image2D::Create(&imageInfo).As<VulkanImage2D>();
				}
				else
				{
					Image2DCreateInfo imageInfo = {};
					imageInfo.Width = m_Specs.Width;
					imageInfo.Height = m_Specs.Height;
					imageInfo.Usage = ImageUsage::Attachment;
					imageInfo.Format = attachment.Format;

					m_AttachmentImages.emplace_back(Image2D::Create(&imageInfo));
				}
			}
			attachmentIndex++;
		}
		HZR_CORE_INFO("Created VulkanFrameBuffer with {0} attachments, has depth {1}", m_AttachmentImages.size(), m_DepthAttachment);
		Resize(m_Specs.Width, m_Specs.Height, true);
	}
	void VulkanFrameBuffer::Resize(uint32_t width, uint32_t height, bool force)
	{
		if (!force && (width == m_Specs.Width && height == m_Specs.Height)) return;

		m_Specs.Width = width;
		m_Specs.Height = height;

		if (!m_Specs.SwapChainTarget) {
			Invalidate();
		}
		else
		{
			Ref<VulkanSwapChain> swapChain = VulkanContext::GetSwapchain();
			m_RenderPass = swapChain->GetRenderPass();
			m_ClearValues.clear();

			glm::vec4 c = VulkanContext::GetClearColor();
			m_ClearValues.emplace_back().color = { c.r, c.g, c.b, c.a };
		}

	}
	void VulkanFrameBuffer::Invalidate()
	{
		RT_Invalidate();
	}
	void VulkanFrameBuffer::RT_Invalidate()
	{
		auto device = VulkanContext::GetDevice();

		if (m_FrameBuffer) {
			vkDestroyFramebuffer(device->GetDevice(), m_FrameBuffer, nullptr);

			if (!m_Specs.pFrameBuffer) {
				uint32_t attachmentIndex = 0;
				for (Ref<VulkanImage2D> image : m_AttachmentImages) {

					image->Release();
					attachmentIndex++;
				}
				if (m_DepthAttachment) {
					m_DepthAttachment->Release();
				}
			}
		}
		VulkanAllocator allocator("FrameBuffer");

		std::vector<VkAttachmentDescription> attachmentDescriptions;
		std::vector<VkAttachmentReference> colorAttachmentReferences;
		VkAttachmentReference depthAttachmentReference;

		m_ClearValues.resize(m_Specs.AttachmentCount);

		bool createImages = m_AttachmentImages.empty();

		if (m_Specs.pFrameBuffer) 
			m_AttachmentImages.clear();

		uint32_t attachmentIndex = 0;

		for (auto attachmentSpec : m_Specs.Attachments) {
			if (VKUtils::IsDepth(attachmentSpec.Format))
			{

				if (m_Specs.pFrameBuffer)
				{
					Ref<VulkanFrameBuffer> existingFramebuffer = m_Specs.pFrameBuffer.As<VulkanFrameBuffer>();
					m_DepthAttachment = existingFramebuffer->GetDepthImage();
				}
				else
				{
					Ref<VulkanImage2D> depthAttachmentImage = m_DepthAttachment.As<VulkanImage2D>();
					depthAttachmentImage->Resize(GetWidth(), GetHeight());
				}

				VkAttachmentDescription& attachmentDescription = attachmentDescriptions.emplace_back();
				attachmentDescription.flags = 0;
				attachmentDescription.format = VKUtils::GetFormat(attachmentSpec.Format);
				attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
				attachmentDescription.loadOp = m_Specs.ClearOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
				attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // TODO: if sampling, needs to be store (otherwise DONT_CARE is fine)
				attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachmentDescription.initialLayout = m_Specs.ClearOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

				if (attachmentSpec.Format == ImageFormat::DEPTH24STENCIL8 || true) // Separate layouts requires a "separate layouts" flag to be enabled
				{
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // TODO: if not sampling
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL; // TODO: if sampling
					depthAttachmentReference = { attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
				}
				else
				{
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL; // TODO: if not sampling
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL; // TODO: if sampling
					depthAttachmentReference = { attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL };
				}
				m_ClearValues[attachmentIndex].depthStencil = { 1.0f, 0 };
			}
			else
			{
				Ref<VulkanImage2D> colorAttachment;
				if (m_Specs.pFrameBuffer) 
				{
					
				}
				else 
				{
					if (createImages)
					{
						Image2DCreateInfo createInfo;
						createInfo.Format = attachmentSpec.Format;
						createInfo.Usage = ImageUsage::Attachment;
						createInfo.Width = GetWidth();
						createInfo.Height = GetHeight();
						colorAttachment = m_AttachmentImages.emplace_back(Image2D::Create(&createInfo)).As<VulkanImage2D>();
					}
					else 
					{
						Ref<VulkanImage2D> image = m_AttachmentImages[attachmentIndex].As<VulkanImage2D>();
						image->Resize(GetWidth(), GetHeight());
					}
				}

				VkAttachmentDescription& attachmentDescription = attachmentDescriptions.emplace_back();
				attachmentDescription.flags = 0;
				attachmentDescription.format = VKUtils::GetFormat(attachmentSpec.Format);
				attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
				attachmentDescription.loadOp = m_Specs.ClearOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
				attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // TODO: if sampling, needs to be store (otherwise DONT_CARE is fine)
				attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachmentDescription.initialLayout = m_Specs.ClearOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

				const auto& clearColor = m_Specs.ClearColor;
				m_ClearValues[attachmentIndex].color = { {clearColor.r, clearColor.g, clearColor.b, clearColor.a} };
				colorAttachmentReferences.emplace_back(VkAttachmentReference{ attachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
			}
			attachmentIndex++;
		}

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = uint32_t(colorAttachmentReferences.size());
		subpassDescription.pColorAttachments = colorAttachmentReferences.data();

		if (m_DepthAttachment)
			subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;

		std::vector<VkSubpassDependency> dependencies;

		if (m_AttachmentImages.size())
		{
			{
				VkSubpassDependency& depedency = dependencies.emplace_back();
				depedency.srcSubpass = VK_SUBPASS_EXTERNAL;
				depedency.dstSubpass = 0;
				depedency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				depedency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				depedency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				depedency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
			{
				VkSubpassDependency& depedency = dependencies.emplace_back();
				depedency.srcSubpass = 0;
				depedency.dstSubpass = VK_SUBPASS_EXTERNAL;
				depedency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				depedency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				depedency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				depedency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
		}
		if (m_DepthAttachment)
		{
			{
				VkSubpassDependency& depedency = dependencies.emplace_back();
				depedency.srcSubpass = VK_SUBPASS_EXTERNAL;
				depedency.dstSubpass = 0;
				depedency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				depedency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
				depedency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				depedency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}

			{
				VkSubpassDependency& depedency = dependencies.emplace_back();
				depedency.srcSubpass = 0;
				depedency.dstSubpass = VK_SUBPASS_EXTERNAL;
				depedency.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
				depedency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				depedency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				depedency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
		}
		// Create the actual renderpass
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = uint32_t(attachmentDescriptions.size());
		renderPassInfo.pAttachments = attachmentDescriptions.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		VK_CHECK_RESULT(vkCreateRenderPass(device->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass));

		std::vector<VkImageView> attachments(m_AttachmentImages.size());

		for (uint32_t i = 0; i < m_AttachmentImages.size(); i++)
		{
			Ref<VulkanImage2D> image = m_AttachmentImages[i].As<VulkanImage2D>();
			attachments[i] = image->GetImageInfo().ImageView;
		}
		if (m_DepthAttachment)
		{
			attachments.emplace_back(m_DepthAttachment->GetImageInfo().ImageView);
		}

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = m_RenderPass;
		framebufferCreateInfo.attachmentCount = uint32_t(attachments.size());
		framebufferCreateInfo.pAttachments = attachments.data();
		framebufferCreateInfo.width = GetWidth();
		framebufferCreateInfo.height = GetHeight();
		framebufferCreateInfo.layers = 1;

		VK_CHECK_RESULT(vkCreateFramebuffer(device->GetDevice(), &framebufferCreateInfo, nullptr, &m_FrameBuffer));
	}
}
