
#include "MetalRenderCommandBuffer.h"

#ifdef HZR_INCLUDE_METAL
#include "MetalContext.h"
#include "Backend/Core/Window.h"

namespace HazardRenderer::Metal
{
    MetalRenderCommandBuffer::MetalRenderCommandBuffer(uint32_t count, const std::string& name, bool compute)
    {
        
    }
    MetalRenderCommandBuffer::MetalRenderCommandBuffer(const std::string& name, bool swapchain)
    {
        auto device = MetalContext::GetInstance()->GetDevice();
    }
    MetalRenderCommandBuffer::~MetalRenderCommandBuffer()
    {
        
    }
}
#endif
