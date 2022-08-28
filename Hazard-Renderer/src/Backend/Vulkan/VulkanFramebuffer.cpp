
#include "VulkanFramebuffer.h"
#include "VulkanContext.h"
#include "Backend/Core/Renderer.h"

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
		if (!force && (m_Specs.Width == width && m_Specs.Height == height)) return;

		m_Specs.Width = width;
		m_Specs.Height = height;

		if (!m_Specs.SwapChainTarget) 
		{
			Invalidate();
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
	void VulkanFrameBuffer::Invalidate()
	{
		Ref<VulkanFrameBuffer> instance = this;
		Renderer::Submit([instance]() mutable {
			instance->Invalidate_RT();
			});
	}
	void VulkanFrameBuffer::Invalidate_RT()
	{
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		Release();

		VulkanAllocator allocator("VulkanFramebuffer");

		std::vector<VkAttachmentDescription> attachmentDescriptions;
		std::vector<VkAttachmentReference> attachmentReferences;
		VkAttachmentReference depthReference;

		m_ClearValues.resize(m_Specs.AttachmentCount);
		
	}
}
