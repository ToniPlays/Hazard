
#include <hzrpch.h>
#include "MetalRenderCommandBuffer.h"
#include "MetalContext.h"


namespace Hazard::Rendering::Metal
{
    MetalRenderCommandBuffer::MetalRenderCommandBuffer(uint32_t size, const std::string& name)
    {
        m_CommandBuffers.reserve(size);
        MTL::CommandQueue* queue = MetalContext::GetMetalCommandQueue();
        
        for(uint32_t i = 0; i < size; i++) {
            m_CommandBuffers[i] = queue->commandBuffer();
        }
    }
    MetalRenderCommandBuffer::MetalRenderCommandBuffer(const std::string& name, bool swapchain)
    {
        HZR_CORE_INFO("Generating command buffers");
    }
}
