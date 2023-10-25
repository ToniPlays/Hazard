
#include "VulkanRenderPass.h"
#ifdef HZR_INCLUDE_VULKAN

namespace HazardRenderer::Vulkan 
{
	VulkanRenderPass::VulkanRenderPass(RenderPassCreateInfo* createInfo)
	{
		m_Spec.DebugName = createInfo->DebugName;
		m_Spec.TargetFrameBuffer = createInfo->pTargetFrameBuffer;
	}
	Ref<RenderPass> VulkanRenderPass::Create(RenderPassCreateInfo* info)
	{
		return Ref<VulkanRenderPass>::Create(info);
	}
}
#endif