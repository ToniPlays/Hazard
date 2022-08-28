#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL

#include "Backend/Core/FrameBuffer.h"
#include "Textures/VulkanImage2D.h"

#include <functional>

namespace HazardRenderer::Vulkan
{
	class VulkanFrameBuffer : public FrameBuffer {
	public:
		VulkanFrameBuffer(FrameBufferCreateInfo* info);
		~VulkanFrameBuffer();

		void Resize(uint32_t width, uint32_t height, bool force = false) override;
		void Resize_RT(uint32_t width, uint32_t height, bool force = false) override;

		void Release();
		void Release_RT();
		void Invalidate();
		void Invalidate_RT();

		uint32_t GetWidth() const override { return m_Specs.Width; };
		uint32_t GetHeight() const override { return m_Specs.Height; };
		size_t GetColorAttachmentCount() override { return m_ColorAttachments.size(); }

		Ref<Image2D> GetImage(uint32_t index = 0) const override { return m_ColorAttachments[index]; };
		Ref<Image2D> GetDepthImage() const override { return m_DepthAttachmentImage; }
		FrameBufferSpecification& GetSpecification() override { return m_Specs; }

		//Vulkan specific
		VkFramebuffer GetVulkanFramebuffer() const { return m_Framebuffer; }
		VkRenderPass GetRenderPass() const { return m_RenderPass; }

		std::vector<VkClearValue> GetClearValues() { return m_ClearValues; }

		bool HasDepthAttachment() { return m_DepthAttachmentImage; }

		void AddResizeCallback(const std::function<void(Ref<VulkanFrameBuffer>)> callback) 
		{
			m_ResizeCallbacks.push_back(callback);
		}

	private:

		FrameBufferSpecification m_Specs;
		std::vector<Ref<VulkanImage2D>> m_ColorAttachments;
		FrameBufferAttachment m_DepthAttachment;

		Ref<VulkanImage2D> m_DepthAttachmentImage;
		Ref<VulkanImage2D> m_ExistingImage;
		std::map<uint32_t, Ref<VulkanImage2D>> m_ExistingImages;
		std::vector<uint32_t> m_ExistingImageLayers;
		std::vector<VkClearValue> m_ClearValues;

		VkFramebuffer m_Framebuffer = VK_NULL_HANDLE;
		VkRenderPass m_RenderPass = VK_NULL_HANDLE;

		std::vector<std::function<void(Ref<VulkanFrameBuffer>)>> m_ResizeCallbacks;
	};
}
#endif
