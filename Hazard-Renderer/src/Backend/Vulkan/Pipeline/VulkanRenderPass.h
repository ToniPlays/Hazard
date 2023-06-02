#pragma once

#include "Backend/Core/RenderPass.h"
#ifdef HZR_INCLUDE_VULKAN

namespace HazardRenderer::Vulkan 
{
	class VulkanRenderPass : public RenderPass 
	{
	public:
		VulkanRenderPass(RenderPassCreateInfo* createInfo);
		~VulkanRenderPass() = default;

		virtual RenderPassSpecification GetSpecs() { return m_Spec; };
		virtual const RenderPassSpecification GetSpecs() const { return m_Spec; };

		static Ref<RenderPass> Create(RenderPassCreateInfo* info);

	private:
		RenderPassSpecification m_Spec;
	};
}
#endif
