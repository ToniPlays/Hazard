

#include "VulkanRenderPass.h"
#ifdef HZR_INCLUDE_VULKAN

namespace HazardRenderer::Vulkan
{
	VulkanRenderPass::VulkanRenderPass(RenderPassCreateInfo* info)
	{
		m_Specs.DebugName = info->DebugName;
		m_Specs.TargetFrameBuffer = info->pTargetFrameBuffer;
	}
	VulkanRenderPass::~VulkanRenderPass()
	{

	}
}
#endif
