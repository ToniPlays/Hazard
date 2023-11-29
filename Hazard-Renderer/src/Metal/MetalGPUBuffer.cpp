
#include "MetalGPUBuffer.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalContext.h"
#include "MetalRenderCommandBuffer.h"
#include "Core/Renderer.h"
#include "MTLUtils.h"

#include "MathCore.h"

namespace HazardRenderer::Metal
{
    MetalGPUBuffer::MetalGPUBuffer(BufferCreateInfo* info)
    {
        m_DebugName = info->Name;
        m_Size = info->Size;
        
        Ref<MetalGPUBuffer> instance = this;
        Renderer::SubmitResourceCreate([instance]() mutable {
            auto device = MetalContext::GetMetalDevice();
            instance->m_Buffer = device->GetMetalDevice()->newBuffer(instance->m_Size, MTL::ResourceOptionCPUCacheModeDefault);
                        
            SetDebugLabel(instance->m_Buffer, instance->m_DebugName);
        });
        
        if (info->Data && !(info->UsageFlags & BUFFER_USAGE_DYNAMIC))
        {
            BufferCopyRegion region = {};
            region.Data = info->Data;
            region.Size = info->Size;
            region.Offset = 0;

            Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::Create("Upload buffer", DeviceQueue::TransferBit, 1);
            cmdBuffer->Begin();
            cmdBuffer->CopyToBuffer(this, region);
            cmdBuffer->End();
            cmdBuffer->Submit();
        }
        else if(info->Data)
        {
            BufferCopyRegion region = {};
            region.Data = info->Data;
            region.Size = info->Size;
            region.Offset = 0;

            SetData(region);
        }
    }
    MetalGPUBuffer::~MetalGPUBuffer()
    {
        m_LocalBuffer.Release();
        Renderer::SubmitResourceFree([buffer = m_Buffer]() mutable {
            buffer->release();
        });
    }
    void MetalGPUBuffer::SetData(const BufferCopyRegion& copyRegion)
    {
        Ref<MetalGPUBuffer> instance = this;
        m_LocalBuffer.Allocate(copyRegion.Size);
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
