
#include "MetalBuffers.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalContext.h"
#include "MetalRenderCommandBuffer.h"
#include "Renderer.h"
#include "../MTLUtils.h"

#include "MathCore.h"

namespace HazardRenderer::Metal
{
    MetalVertexBuffer::MetalVertexBuffer(VertexBufferCreateInfo* info)
    {
        m_DebugName = info->DebugName;
        m_Size = info->Size;
        m_Layout = *info->Layout;
        m_Usage = info->Usage;
        
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
    MetalVertexBuffer::~MetalVertexBuffer()
    {
        m_LocalBuffer.Release();
        m_Buffer->release();
    }
    void MetalVertexBuffer::SetData(const BufferCopyRegion& copyRegion)
    {
        Ref<MetalVertexBuffer> instance = this;
        m_LocalBuffer = Buffer::Copy(copyRegion.Data, copyRegion.Size);
        Renderer::Submit([instance, copyRegion]() mutable {
            instance->SetData_RT(copyRegion);
        });
    }
    void MetalVertexBuffer::SetData_RT(const BufferCopyRegion& copyRegion)
    {
        void* data = m_Buffer->contents();
        memcpy((uint8_t*)data + copyRegion.Offset, m_LocalBuffer.Data, m_LocalBuffer.Size);
        
        m_LocalBuffer.Release();
    }

    
    MetalIndexBuffer::MetalIndexBuffer(IndexBufferCreateInfo* info)
    {
        m_DebugName = info->DebugName;
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

    MetalUniformBuffer::MetalUniformBuffer(UniformBufferCreateInfo* info)
    {
        m_Name = info->Name;
        m_Binding = info->Binding;
        m_Size = Math::Max<size_t>(256, info->Size);
        m_Usage = info->Usage;
        m_LocalData.Allocate(m_Size * 1024);

        Ref<MetalUniformBuffer> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            instance->Invalidate_RT();
            });
    }
    MetalUniformBuffer::~MetalUniformBuffer()
    {
        m_LocalData.Release();
        m_Buffer->release();
    }
    void MetalUniformBuffer::SetData(const BufferCopyRegion& copyRegion)
    {
        m_LocalData = Buffer::Copy(copyRegion.Data, copyRegion.Size);
        Ref<MetalUniformBuffer> instance = this;
        Renderer::Submit([instance]() mutable {
            void* contents = instance->m_Buffer->contents();
            memcpy(contents, instance->m_LocalData.Data, instance->m_LocalData.Size);
        });
    }
    void MetalUniformBuffer::Invalidate_RT()
    {
        auto device = MetalContext::GetMetalDevice();
        
        m_Buffer = device->GetMetalDevice()->newBuffer(m_Size, MTL::ResourceOptionCPUCacheModeWriteCombined);
        
        SetDebugLabel(m_Buffer, m_Name);
    }
}
#endif
