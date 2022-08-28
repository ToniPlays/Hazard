
#include "VulkanFramebuffer.h"
#ifdef HZR_INCLUDE_VULKAN
#include "VulkanContext.h"
#include "Backend/Core/Renderer.h"
#include "VkUtils.h"


#include "spdlog/fmt/fmt.h"


namespace HazardRenderer::Vulkan
{
	VulkanFrameBuffer::VulkanFrameBuffer(FrameBufferCreateInfo* info)
	{
		m_Specs.DebugName = info->DebugName;
		m_Specs.AttachmentCount = info->AttachmentCount;
		m_Specs.ClearColor = info->ClearColor;
		m_Specs.Attachments = info->Attachments;
		m_Specs.pFrameBuffer = info->pFrameBuffer;
		m_Specs.SwapChainTarget = info->SwapChainTarget;
		m_Specs.ClearColor = info->ClearColor;

		if (info->Width == 0)
		{
			auto swapchain = VulkanContext::GetInstance()->GetSwapchain();
			m_Specs.Width = swapchain->GetWidth();
			m_Specs.Height = swapchain->GetHeight();
		}
		else
		{
			m_Specs.Width = info->Width;
			m_Specs.Height = info->Height;
		}

		//Create images immediate
		uint32_t attachmentIndex = 0;
		if (!info->pFrameBuffer)
		{
			for (auto& attachmentSpec : m_Specs.Attachments)
			{
				if (m_ExistingImage && m_ExistingImage->GetLayerCount() > 1)
				{
					if (attachmentSpec.IsDepth())
						m_DepthAttachmentImage = m_ExistingImage;
					else m_ColorAttachments.emplace_back(m_ExistingImage);
				}
				else if (m_ExistingImages.find(attachmentIndex) != m_ExistingImages.end())
				{
					if (!attachmentSpec.IsDepth())
						m_ColorAttachments.emplace_back();
				}
				else if (attachmentSpec.IsDepth())
				{
					Image2DCreateInfo imageInfo = {};
					imageInfo.Format = attachmentSpec.Format;
					imageInfo.Usage = ImageUsage::Attachment;
					imageInfo.Width = m_Specs.Width;
					imageInfo.Height = m_Specs.Height;
					imageInfo.DebugName = fmt::format("{0}-Depth Attachment {1}", m_Specs.DebugName.empty() ? "Unnamed FB" : m_Specs.DebugName, attachmentIndex);
					m_DepthAttachmentImage = Image2D::Create(&imageInfo).As<VulkanImage2D>();
				}
				else
				{
					Image2DCreateInfo imageInfo = {};
					imageInfo.Format = attachmentSpec.Format;
					imageInfo.Usage = ImageUsage::Attachment;
					imageInfo.Width = m_Specs.Width;
					imageInfo.Height = m_Specs.Height;
					imageInfo.DebugName = fmt::format("{0}-Color Attachment {1}", m_Specs.DebugName.empty() ? "Unnamed FB" : m_Specs.DebugName, attachmentIndex);
					m_ColorAttachments.emplace_back(Image2D::Create(&imageInfo).As<VulkanImage2D>());
				}
				attachmentIndex++;
			}
		}
		Resize(m_Specs.Width, m_Specs.Height, true);
	}
	VulkanFrameBuffer::~VulkanFrameBuffer()
	{
		Release();
	}
	void VulkanFrameBuffer::Resize(uint32_t width, uint32_t height, bool force)
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanFrameBuffer> instance = this;
		Renderer::Submit([instance, width, height, force]() mutable {
			instance->Resize_RT(width, height, force);
			});
	}
	void VulkanFrameBuffer::Resize_RT(uint32_t width, uint32_t height, bool force)
	{
		HZR_PROFILE_FUNCTION();
		if (!force && (m_Specs.Width == width && m_Specs.Height == height)) return;

		m_Specs.Width = width;
		m_Specs.Height = height;

		if (!m_Specs.SwapChainTarget)
		{
			Invalidate_RT();
		}
		else
		{
			auto& swapchain = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>();
			m_RenderPass = swapchain->GetVulkanRenderPass();

			m_ClearValues.clear();
			m_ClearValues.emplace_back().color = VulkanContext::GetInstance()->GetClearColorValue();
		}

		for (auto& cb : m_ResizeCallbacks)
		{
			cb(this);
		}
	}
	void VulkanFrameBuffer::Release()
	{
		HZR_PROFILE_FUNCTION();
		if (!m_Framebuffer) return;

		VkFramebuffer instance = m_Framebuffer;
		Renderer::SubmitResourceFree([instance]() mutable {
			const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
			vkDestroyFramebuffer(device, instance, nullptr);
			});

		if (m_Specs.pFrameBuffer) return;

		uint32_t attachmentIndex = 0;
		for (auto& image : m_ColorAttachments)
		{
			if (m_ExistingImages.find(attachmentIndex) != m_ExistingImages.end()) continue;

			if (image->GetLayerCount() == 1 || attachmentIndex == 0 && !image->GetLayerImageView(0))
				image->Release();
			attachmentIndex++;
		}
		if (m_DepthAttachmentImage)
		{
			if (m_ExistingImages.find((uint32_t)m_Specs.Attachments.size() - 1) == m_ExistingImages.end())
				m_DepthAttachmentImage->Release();
		}
	}
	void VulkanFrameBuffer::Release_RT()
	{
		HZR_PROFILE_FUNCTION();
		if (!m_Framebuffer) return;

		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		vkDeviceWaitIdle(device);

		vkDestroyFramebuffer(device, m_Framebuffer, nullptr);

		if (m_Specs.pFrameBuffer) return;

		uint32_t attachmentIndex = 0;
		for (auto& image : m_ColorAttachments)
		{
			if (m_ExistingImages.find(attachmentIndex) != m_ExistingImages.end()) continue;

			if (image->GetLayerCount() == 1 || attachmentIndex == 0 && !image->GetLayerImageView(0))
				image->Release_RT();
			attachmentIndex++;
		}
		if (m_DepthAttachmentImage)
		{
			if (m_ExistingImages.find((uint32_t)m_Specs.Attachments.size() - 1) == m_ExistingImages.end())
				m_DepthAttachmentImage->Release_RT();
		}
	}
	void VulkanFrameBuffer::Invalidate()
	{
		HZR_PROFILE_FUNCTION();
		Ref<VulkanFrameBuffer> instance = this;
		Renderer::Submit([instance]() mutable {
			instance->Invalidate_RT();
			});
	}
	void VulkanFrameBuffer::Invalidate_RT()
	{
		HZR_PROFILE_FUNCTION();
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		Release_RT();

		VulkanAllocator allocator("VulkanFramebuffer");

		std::vector<VkAttachmentDescription> attachmentDescriptions;
		std::vector<VkAttachmentReference> attachmentReferences;

		std::vector<VkAttachmentReference> colorReferences;
		VkAttachmentReference depthReference;

		m_ClearValues.resize(m_Specs.AttachmentCount);

		bool createImages = m_ColorAttachments.empty();

		if (m_Specs.pFrameBuffer)
			m_ColorAttachments.clear();

		uint32_t attachmentImageIndex = 0;
		for (auto spec : m_Specs.Attachments)
		{
			if (spec.IsDepth())
			{
				if (m_ExistingImage)
				{
					m_DepthAttachmentImage = m_ExistingImage;
				}
				else if (m_Specs.pFrameBuffer)
				{
					Ref<VulkanFrameBuffer> targetBuffer = m_Specs.pFrameBuffer.As<VulkanFrameBuffer>();
					m_DepthAttachmentImage = targetBuffer->GetDepthImage().As<VulkanImage2D>();
				}
				else if (m_ExistingImages.find(attachmentImageIndex) != m_ExistingImages.end())
				{
					Ref<VulkanImage2D> existingImage = m_ExistingImages[attachmentImageIndex];
					HZR_ASSERT(existingImage->GetFormat() >= ImageFormat::DEPTH32F, "Color image cannot be depth image");
					m_DepthAttachmentImage = existingImage;
				}
				else
				{
					m_DepthAttachmentImage->Resize_RT(m_Specs.Width, m_Specs.Height);
				}

				VkAttachmentDescription& attachmentDescription = attachmentDescriptions.emplace_back();
				attachmentDescription.flags = 0;
				attachmentDescription.format = VkUtils::VulkanImageFormat(spec.Format);
				attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
				attachmentDescription.loadOp = m_Specs.ClearOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
				attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

				attachmentDescription.initialLayout = m_Specs.ClearOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR;

				if (spec.Format == ImageFormat::DEPTH24STENCIL8 || true)
				{
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					depthReference = { attachmentImageIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
				}
				else
				{
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
					depthReference = { attachmentImageIndex, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL };
				}
				m_ClearValues[attachmentImageIndex].depthStencil = { 0.0f, 0 };
			}
			else
			{
				Ref<VulkanImage2D> colorAttachment;
				if (m_Specs.pFrameBuffer)
				{
					Ref<VulkanFrameBuffer> targetBuffer = m_Specs.pFrameBuffer.As<VulkanFrameBuffer>();
					Ref<VulkanImage2D> image = targetBuffer->GetImage(attachmentImageIndex).As<VulkanImage2D>();
					colorAttachment = m_ColorAttachments.emplace_back(image);
				}
				else if (m_ExistingImages.find(attachmentImageIndex) != m_ExistingImages.end())
				{
					Ref<VulkanImage2D> image = m_ExistingImages[attachmentImageIndex];
					HZR_ASSERT(image->GetFormat() >= ImageFormat::DEPTH32F, "Color image cannot be depth image");
					m_ColorAttachments[attachmentImageIndex] = image;
				}
				else
				{
					if (createImages)
					{
						Image2DCreateInfo imageInfo = {};
						imageInfo.Format = spec.Format;
						imageInfo.Usage = ImageUsage::Attachment;
						imageInfo.Width = m_Specs.Width;
						imageInfo.Height = m_Specs.Height;

						colorAttachment = m_ColorAttachments.emplace_back(Image2D::Create(&imageInfo).As<VulkanImage2D>());
					}
					else
					{
						colorAttachment = m_ColorAttachments[attachmentImageIndex];
						if (colorAttachment->GetLayerCount() == 1)
							colorAttachment->Resize_RT(m_Specs.Width, m_Specs.Height);
						else if (colorAttachment->GetLayerCount() == 0 && m_ExistingImageLayers[0] == 0)
						{
							colorAttachment->Invalidate_RT();
							__debugbreak();
						}
						else if (attachmentImageIndex == 0)
						{
							__debugbreak();
						}
					}

					VkAttachmentDescription& attachmentDescription = attachmentDescriptions.emplace_back();
					attachmentDescription.flags = 0;
					attachmentDescription.format = VkUtils::VulkanImageFormat(spec.Format);
					attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
					attachmentDescription.loadOp = m_Specs.ClearOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
					attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
					attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

					attachmentDescription.initialLayout = m_Specs.ClearOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

					const auto& color = m_Specs.ClearColor;
					m_ClearValues[attachmentImageIndex].color = { color.r, color.g, color.b, color.a };
					colorReferences.emplace_back(VkAttachmentReference{ attachmentImageIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
				}
			}
			attachmentImageIndex++;
		}
		VkSubpassDescription subpassDesc = {};
		subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDesc.colorAttachmentCount = (uint32_t)colorReferences.size();
		subpassDesc.pColorAttachments = colorReferences.data();

		if (m_DepthAttachmentImage)
			subpassDesc.pDepthStencilAttachment = &depthReference;

		std::vector<VkSubpassDependency> dependencies;

		if (m_ColorAttachments.size())
		{
			{
				VkSubpassDependency& dependency = dependencies.emplace_back();
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = 0;
				dependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				dependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
			{
				VkSubpassDependency& dependency = dependencies.emplace_back();
				dependency.srcSubpass = 0;
				dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
				dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				dependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
		}
		if (m_DepthAttachmentImage)
		{
			{
				VkSubpassDependency& dependency = dependencies.emplace_back();
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = 0;
				dependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				dependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
				dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
			{
				VkSubpassDependency& dependency = dependencies.emplace_back();
				dependency.srcSubpass = 0;
				dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
				dependency.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
				dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				dependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
		}

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = (uint32_t)attachmentDescriptions.size();
		renderPassInfo.pAttachments = attachmentDescriptions.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDesc;
		renderPassInfo.dependencyCount = (uint32_t)dependencies.size();
		renderPassInfo.pDependencies = dependencies.data();

		VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_RenderPass), "Failed to create VkRenderPass");
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_RENDER_PASS, m_Specs.DebugName, m_RenderPass);

		std::vector<VkImageView> attachmentViews(m_ColorAttachments.size());
		for (uint32_t i = 0; i < m_ColorAttachments.size(); i++)
		{
			Ref<VulkanImage2D> image = m_ColorAttachments[i];
			if (image->GetLayerCount() > 1)
			{
				__debugbreak();
			}
			else attachmentViews[i] = image->GetImageInfo().ImageView;
		}
		if (m_DepthAttachmentImage)
		{
			if (m_ExistingImage)
			{
				__debugbreak();
			}
			else
				attachmentViews.emplace_back(m_DepthAttachmentImage->GetImageInfo().ImageView);
		}

		VkFramebufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferInfo.renderPass = m_RenderPass;
		frameBufferInfo.attachmentCount = (uint32_t)attachmentViews.size();
		frameBufferInfo.pAttachments = attachmentViews.data();
		frameBufferInfo.width = m_Specs.Width;
		frameBufferInfo.height = m_Specs.Height;
		frameBufferInfo.layers = 1;

		VK_CHECK_RESULT(vkCreateFramebuffer(device, &frameBufferInfo, nullptr, &m_Framebuffer), "Failed to create Vulkan Framebuffer");
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_FRAMEBUFFER, m_Specs.DebugName, m_Framebuffer);
	}
}
#endif