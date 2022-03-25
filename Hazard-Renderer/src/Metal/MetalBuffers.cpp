
#include "MetalBuffers.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalContext.h"
#include "MetalRenderCommandBuffer.h"

namespace HazardRenderer::Metal
{
    MetalVertexBuffer::MetalVertexBuffer(VertexBufferCreateInfo* createInfo)
    {
        MTL::Device* device = MetalContext::GetMetalDevice();
        m_Buffer = device->newBuffer(createInfo->Size, NULL);
        
        m_DebugName = createInfo->DebugName;
        
        if(createInfo->Data)
        {
            SetData(createInfo->Data, createInfo->Size);
        }
        HZR_CORE_INFO(m_Buffer->allocatedSize());
    }
    MetalVertexBuffer::~MetalVertexBuffer()
    {
        m_Buffer = nullptr;
    }
    void MetalVertexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
    {
        auto mtCmdBuffer = cmdBuffer.As<MetalRenderCommandBuffer>();
        auto encoder = mtCmdBuffer->GetEncoder();
        encoder->setVertexBuffer(m_Buffer, 0, 0);
    }
    
    void MetalVertexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
    {
        //auto mtCmdBuffer = cmdBuffer.As<MetalRenderCommandBuffer>();
        //auto encoder = mtCmdBuffer->GetEncoder();
    }

    void MetalVertexBuffer::SetData(const void *data, uint32_t size) {
        //void* contents = m_Buffer->contents();
        //memcpy(contents, data, size);
    }
    MetalIndexBuffer::MetalIndexBuffer(IndexBufferCreateInfo* createInfo)
    {
        MTL::Device* device = MetalContext::GetMetalDevice();
        m_Buffer = device->newBuffer(createInfo->Size * sizeof(uint32_t), NULL);
        
        m_DebugName = createInfo->DebugName;
    }
    MetalIndexBuffer::~MetalIndexBuffer()
    {
        m_Buffer = nullptr;
    }
}
#endif