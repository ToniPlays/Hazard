
#include "MetalBuffers.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalContext.h"
#include "MetalRenderCommandBuffer.h"

namespace HazardRenderer::Metal
{
    MetalVertexBuffer::MetalVertexBuffer(VertexBufferCreateInfo* createInfo)
    {
        
        MTL::Device* device = MetalContext::GetMetalDevice();
        m_Buffer = device->newBuffer(createInfo->Size, MTL::ResourceOptionCPUCacheModeDefault);
        m_DebugName = createInfo->DebugName;
        m_Size = createInfo->Size;
        
        if (createInfo->Layout != nullptr)
            m_Layout = *createInfo->Layout;
        
        if(createInfo->Data)
        {
            SetData(createInfo->Data, createInfo->Size);
        }
    }
    MetalVertexBuffer::~MetalVertexBuffer()
    {
        m_Buffer = nullptr;
    }
    void MetalVertexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer, uint32_t binding)
    {
        auto mtCmdBuffer = cmdBuffer.As<MetalRenderCommandBuffer>();
        auto encoder = mtCmdBuffer->GetEncoder();
        encoder->setVertexBuffer(m_Buffer, 0, binding);
    }
    
    void MetalVertexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
    {
        
    }

    void MetalVertexBuffer::SetData(const void *data, uint32_t size) {
        void* contents = m_Buffer->contents();
        memcpy(contents, data, size);
    }
    MetalIndexBuffer::MetalIndexBuffer(IndexBufferCreateInfo* createInfo)
    {
        MTL::Device* device = MetalContext::GetMetalDevice();
        m_Buffer = device->newBuffer(createInfo->Size * sizeof(uint32_t), NULL);
        m_Size = createInfo->Size;
        
        m_DebugName = createInfo->DebugName;
        
        SetData(createInfo->Data, createInfo->Size);
        
    }
    MetalIndexBuffer::~MetalIndexBuffer()
    {
        m_Buffer->release();
    }
    void MetalIndexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer) {
        cmdBuffer.As<MetalRenderCommandBuffer>()->BindIndexBuffer(m_Buffer);
    }
    void MetalIndexBuffer::SetData(uint32_t *data, uint32_t size) {
        void* contents = m_Buffer->contents();
        memcpy(contents, data, size);
    }
}
#endif
