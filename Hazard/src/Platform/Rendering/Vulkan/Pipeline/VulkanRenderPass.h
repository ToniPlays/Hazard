#pragma once

#include "Hazard/Rendering/Pipeline/FrameBuffer.h"
#include "Hazard/Rendering/Pipeline/RenderPass.h"
#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan {
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