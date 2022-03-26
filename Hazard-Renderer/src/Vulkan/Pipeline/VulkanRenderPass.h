#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Core/FrameBuffer.h"
#include "Core/Pipeline/RenderPass.h"

namespace HazardRenderer::Vulkan {
	class VulkanRenderPass : public RenderPass {
	public:
		VulkanRenderPass(RenderPassCreateInfo* info);
		~VulkanRenderPass();

		virtual RenderPassSpecification GetSpecs() { return m_Specs; };
		virtual const RenderPassSpecification GetSpecs() const { return m_Specs; };

	private:
		RenderPassSpecification m_Specs;
	};
}
#endif