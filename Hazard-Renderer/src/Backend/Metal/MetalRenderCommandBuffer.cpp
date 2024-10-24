
#include "MetalRenderCommandBuffer.h"
#ifdef HZR_INCLUDE_METAL
#include "MetalContext.h"
#include "Backend/Core/Window.h"

namespace HazardRenderer::Metal
{
    MetalRenderCommandBuffer::MetalRenderCommandBuffer(uint32_t size, const std::string& name)
    {
        Window::SendDebugMessage({ Severity::Info, "Creating command buffer for: " + name });
    }
    MetalRenderCommandBuffer::MetalRenderCommandBuffer(const std::string& name, bool swapchain) : m_OwnedBySwapchain((swapchain))
    {
        Window::SendDebugMessage({ Severity::Info, "Generating command buffers" });

    }
    void MetalRenderCommandBuffer::Begin()
    {
        m_CommandBuffer = MetalContext::GetMetalCommandQueue()->commandBuffer();
    }
    void MetalRenderCommandBuffer::End()
    {
    }
    void MetalRenderCommandBuffer::Submit()
    {
        if(m_OwnedBySwapchain) {
            return;
        }
        
        m_CommandBuffer->commit();
    }

    void MetalRenderCommandBuffer::BeginRenderEncoder(MTL::RenderPassDescriptor* descriptor)
    {
        m_Encoder = m_CommandBuffer->renderCommandEncoder(descriptor);
    }
}
#endif
