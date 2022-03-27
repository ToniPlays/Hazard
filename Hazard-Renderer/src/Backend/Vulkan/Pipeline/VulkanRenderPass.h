#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/FrameBuffer.h"
#include "Backend/Core/Pipeline/RenderPass.h"

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