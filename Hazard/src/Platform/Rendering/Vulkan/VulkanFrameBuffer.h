#pragma once

#include "Hazard/Rendering/Pipeline/FrameBuffer.h"
#include "Image/VulkanImage2D.h"
#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan {
	class VulkanFrameBuffer : public FrameBuffer {
	public:
		VulkanFrameBuffer(FrameBufferCreateInfo* info);

		virtual void Bind() const {};
		virtual void Unbind() const {};

		virtual void Resize(uint32_t width, uint32_t height, bool force = false);
		virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const {};

		virtual uint32_t GetWidth() const { return m_Specs.Width; };
		virtual uint32_t GetHeight() const { return m_Specs.Height; };

		virtual uint32_t GetID() const { return 0; };
		virtual FrameBufferSpecification& GetSpecification() override { return m_Specs; }
		virtual VkRenderPass GetRenderPass() { return m_RenderPass; };
		virtual VkFramebuffer GetFrameBuffer() { return m_FrameBuffer; };
		virtual Ref<VulkanImage2D> GetDepthImage() { return m_DepthAttachment; }
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