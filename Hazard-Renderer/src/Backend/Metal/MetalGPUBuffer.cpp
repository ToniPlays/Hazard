
#include "MetalGPUBuffer.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalContext.h"
#include "MetalRenderCommandBuffer.h"
#include "Backend/Core/Renderer.h"
#include "MTLUtils.h"

#include "MathCore.h"

namespace HazardRenderer::Metal
{
    MetalGPUBuffer::MetalGPUBuffer(BufferCreateInfo* info)
    {
        m_DebugName = info->Name;
        m_Size = info->Size;
        
        m_LocalBuffer.Allocate(m_Size);
        
        HZR_ASSERT(m_Size, "Buffer size cannot be 0");
        
        if(info->Data)
        {
            m_LocalBuffer = Buffer::Copy(info->Data, info->Size);
            Ref<MetalVertexBuffer> instance = this;
            Renderer::SubmitResourceCreate([instance]() mutable {
                
                auto device = MetalContext::GetMetalDevice();
                instance->m_Buffer = device->GetMetalDevice()->newBuffer(instance->m_LocalBuffer.Data, instance->m_Size, MTL::ResourceOptionCPUCacheModeDefault);
                
                SetDebugLabel(instance->m_Buffer, instance->m_DebugName);
            });
        }
        else
        {
            Ref<MetalVertexBuffer> instance = this;
            Renderer::SubmitResourceCreate([instance]() mutable {
                
                auto device = MetalContext::GetMetalDevice();
                instance->m_Buffer = device->GetMetalDevice()->newBuffer(instance->m_Size, MTL::ResourceOptionCPUCacheModeDefault);
                
                SetDebugLabel(instance->m_Buffer, instance->m_DebugName);
            });
        }
    }
    MetalGPUBuffer::~MetalGPUBuffer()
    {
        m_LocalBuffer.Release();
        m_Buffer->release();
    }
    void MetalGPUBuffer::SetData(const BufferCopyRegion& copyRegion)
    {
        Ref<MetalVertexBuffer> instance = this;
        m_LocalBuffer.Write(copyRegion.Data, copyRegion.Size, copyRegion.Offset);
        Renderer::Submit([instance, copyRegion]() mutable {
            instance->SetData_RT(copyRegion);
        });
    }
    void MetalGPUBuffer::SetData_RT(const BufferCopyRegion& copyRegion)
    {
        void* data = m_Buffer->contents();
        memcpy((uint8_t*)data, m_LocalBuffer.Data, m_LocalBuffer.Size);
        m_LocalBuffer.Release();
    }

}
#endif
