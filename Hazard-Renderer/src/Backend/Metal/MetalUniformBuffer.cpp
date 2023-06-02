
#include "MetalUniformBuffer.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalContext.h"
#include "MetalRenderCommandBuffer.h"
#include "Backend/Core/Renderer.h"
#include "MTLUtils.h"

#include "MathCore.h"

namespace HazardRenderer::Metal
{
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
