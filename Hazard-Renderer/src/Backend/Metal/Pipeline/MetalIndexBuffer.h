#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Pipeline/Buffers.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalIndexBuffer : public IndexBuffer
    {
    public:
        MetalIndexBuffer(IndexBufferCreateInfo* createInfo);
        ~MetalIndexBuffer();
        
        size_t GetCount() const override { return m_Size / sizeof(uint32_t); }
        const size_t GetSize() const override { return m_Size; }
        std::string& GetDebugName() override { return m_DebugName; };
        
        void SetData(const BufferCopyRegion& copyRegion) override;
        
        //Metal specific
        MTL::Buffer* GetMetalBuffer() const { return m_Buffer; }

    private:
        void SetData_RT(const BufferCopyRegion& copyRegion);
        
    private:
        size_t m_Size;
        std::string m_DebugName;
        Buffer m_LocalBuffer;
        
        MTL::Buffer* m_Buffer;
    };
}
#endif
