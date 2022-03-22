#ifdef HZR_PLATFORM_MACOS
#include "MetalRenderPass.h"

namespace Hazard::Rendering::Metal
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