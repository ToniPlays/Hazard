#pragma once

#include "Hazard/Rendering/Pipeline/FrameBuffer.h"
#include "Image/VulkanImage2D.h"
#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan {
	class VulkanFrameBuffer : public FrameBuffer {
	public:
		VulkanFrameBuffer(FrameBufferCreateInfo* info);

		void Bind() const {};
		void Unbind() const {};

		void Resize(uint32_t width, uint32_t height, bool force = false);
		void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const {};

		uint32_t GetWidth() const { return m_Specs.Width; };
		uint32_t GetHeight() const { return m_Specs.Height; };

		uint32_t GetColorAttachmentCount() { return m_AttachmentImages.size(); }

		FrameBufferSpecification& GetSpecification() override { return m_Specs; }
		VkRenderPass GetRenderPass() { return m_RenderPass; };
		VkFramebuffer GetFrameBuffer() { return m_FrameBuffer; };
		Ref<VulkanImage2D> GetDepthImage() { return m_DepthAttachment; }
		Ref<Image2D> GetImage(uint32_t index = 0) const override { return m_AttachmentImages[index].As<Image2D>(); };

		const std::vector<VkClearValue>& GetVulkanClearValues() const { return m_ClearValues; }

		void Invalidate();
		void RT_Invalidate();

	private:
		FrameBufferSpecification m_Specs;
		VkRenderPass m_RenderPass = nullptr;
		VkFramebuffer m_FrameBuffer = nullptr;

		std::vector<VkClearValue> m_ClearValues;
		Ref<VulkanImage2D> m_DepthAttachment;

		std::vector<Ref<VulkanImage2D>> m_AttachmentImages;
	};
}