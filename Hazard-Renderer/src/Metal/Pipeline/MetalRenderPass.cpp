
#include "MetalRenderPass.h"
#ifdef HZR_INCLUDE_METAL

namespace HazardRenderer::Metal
{
    MetalRenderPass::MetalRenderPass(RenderPassCreateInfo* info)
    {
        m_Specs.DebugName = info->DebugName;
        m_Specs.TargetFrameBuffer = info->pTargetFrameBuffer;
    }
    MetalRenderPass::~MetalRenderPass()
    {

    }
}
#endif