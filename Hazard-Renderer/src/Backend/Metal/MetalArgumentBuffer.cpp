#include "MetalArgumentBuffer.h"

#ifdef HZR_INCLUDE_METAL
#include "Backend/Core/Renderer.h"
#include "MetalContext.h"
#include "MTLUtils.h"

namespace HazardRenderer::Metal
{
    MetalArgumentBuffer::MetalArgumentBuffer(ArgumentBufferCreateInfo* createInfo) {
        m_DebugName = createInfo->Name;
        m_Size = createInfo->Size;
        
        if(createInfo->Data)
        {
            m_LocalBuffer = Buffer::Copy(createInfo->Data, createInfo->Size);
            Ref<MetalArgumentBuffer> instance = this;
            Renderer::SubmitResourceCreate([instance]() mutable {
                
                auto device = MetalContext::GetMetalDevice();
                instance->m_Buffer = device->GetMetalDevice()->newBuffer(instance->m_LocalBuffer.Data, instance->m_Size, MTL::ResourceOptionCPUCacheModeDefault);
                
                SetDebugLabel(instance->m_Buffer, instance->m_DebugName);
            });
        }
        else
        {
            Ref<MetalArgumentBuffer> instance = this;
            Renderer::SubmitResourceCreate([instance]() mutable {
                
                auto device = MetalContext::GetMetalDevice();
                instance->m_Buffer = device->GetMetalDevice()->newBuffer(instance->m_Size, MTL::ResourceOptionCPUCacheModeDefault);
                
                SetDebugLabel(instance->m_Buffer, instance->m_DebugName);
            });
        }
    }
    MetalArgumentBuffer::~MetalArgumentBuffer() {
        m_LocalBuffer.Release();
        Renderer::SubmitResourceFree([buffer = m_Buffer]() mutable {
            buffer->release();
        });
    }
    void MetalArgumentBuffer::SetData(const BufferCopyRegion& copyRegion)
    {
        
    }
}
#endif
