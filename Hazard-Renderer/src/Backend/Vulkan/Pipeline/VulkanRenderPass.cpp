
#include "VulkanRenderPass.h"


namespace HazardRenderer::Vulkan 
{
	VulkanRenderPass::VulkanRenderPass(RenderPassCreateInfo* createInfo)
	{
		m_Spec.DebugName = createInfo->DebugName;
		m_Spec.TargetFrameBuffer = createInfo->pTargetFrameBuffer;
	}
	Ref<RenderPass> VulkanRenderPass::Create(RenderPassCreateInfo* info)
	{
		return new VulkanRenderPass(info);
	}
}