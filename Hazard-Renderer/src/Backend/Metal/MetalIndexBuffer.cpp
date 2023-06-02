#include "MetalIndexBuffer.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalContext.h"
#include "MetalRenderCommandBuffer.h"
#include "Backend/Core/Renderer.h"
#include "MTLUtils.h"

#include "MathCore.h"

namespace HazardRenderer::Metal
{
    MetalIndexBuffer::MetalIndexBuffer(IndexBufferCreateInfo* info)
    {
        m_DebugName = info->Name;
        m_Size = info->Size;
        
        if(info->Data)
        {
            m_LocalBuffer = Buffer::Copy(info->Data, info->Size);
            Ref<MetalIndexBuffer> instance = this;
            Renderer::SubmitResourceCreate([instance]() mutable {
                
                auto device = MetalContext::GetMetalDevice();
                instance->m_Buffer = device->GetMetalDevice()->newBuffer(instance->m_LocalBuffer.Data, instance->m_Size, MTL::ResourceOptionCPUCacheModeDefault);
                
                SetDebugLabel(instance->m_Buffer, instance->m_DebugName);
            });
        }
        else
        {
            Ref<MetalIndexBuffer> instance = this;
            Renderer::SubmitResourceCreate([instance]() mutable {
                
                auto device = MetalContext::GetMetalDevice();
                instance->m_Buffer = device->GetMetalDevice()->newBuffer(instance->m_Size, MTL::ResourceOptionCPUCacheModeDefault);
                
                SetDebugLabel(instance->m_Buffer, instance->m_DebugName);
            });
        }
    }
    MetalIndexBuffer::~MetalIndexBuffer()
    {
        m_LocalBuffer.Release();
        m_Buffer->release();
    }
    void MetalIndexBuffer::SetData(const BufferCopyRegion &copyRegion)
    {
        Ref<MetalIndexBuffer> instance = this;
        m_LocalBuffer = Buffer::Copy(copyRegion.Data, copyRegion.Size);
        
        Renderer::Submit([instance, copyRegion]() mutable {
            instance->SetData_RT(copyRegion);
        });
    }
    void MetalIndexBuffer::SetData_RT(const BufferCopyRegion &copyRegion)
    {
        void* data = m_Buffer->contents();
        memcpy((uint8_t*)data + copyRegion.Offset, m_LocalBuffer.Data, m_LocalBuffer.Size);
        
        m_LocalBuffer.Release();
    }
}
#endif
