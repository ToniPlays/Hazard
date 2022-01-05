
#include <hzrpch.h>
#include "MetalRenderCommandBuffer.h"
#include "MetalContext.h"

namespace Hazard::Rendering::Metal
{
    MetalRenderCommandBuffer::MetalRenderCommandBuffer(uint32_t size, const std::string& name)
    {
        HZR_CORE_INFO("Creating command buffer for {0}", name);
    }
    MetalRenderCommandBuffer::MetalRenderCommandBuffer(const std::string& name, bool swapchain) : m_OwnedBySwapchain((swapchain))
    {
        HZR_CORE_INFO("Generating command buffers");
    }
    void MetalRenderCommandBuffer::Begin()
    {
        MTL::CommandQueue* queue = MetalContext::GetMetalCommandQueue();
        m_CommandBuffer = queue->commandBuffer();
    }
    void MetalRenderCommandBuffer::End()
    {
        
    }
    void MetalRenderCommandBuffer::Submit()
    {
        if(m_OwnedBySwapchain) {}
        MetalContext::Present(m_CommandBuffer);
        m_CommandBuffer->commit();
    }
    void MetalRenderCommandBuffer::BeginRenderEncoder(MTL::RenderPassDescriptor* descriptor)
    {
        m_Encoder = m_CommandBuffer->renderCommandEncoder(descriptor);
    }
}
