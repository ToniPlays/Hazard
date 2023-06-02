#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Buffers.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalVertexBuffer : public VertexBuffer
    {
    public:
        MetalVertexBuffer(VertexBufferCreateInfo* createInfo);
        ~MetalVertexBuffer();
        
        void SetData(const BufferCopyRegion& copyRegion) override;
        void SetData_RT(const BufferCopyRegion& copyRegion);
        const size_t GetSize() const override { return m_Size; }
        std::string& GetDebugName() override { return m_DebugName; };
        const BufferLayout& GetLayout() const override { return m_Layout; }
        
        //Metal specific
        MTL::Buffer* GetMetalBuffer() const { return m_Buffer; }
        
    private:
        size_t m_Size;
        std::string m_DebugName;
        BufferLayout m_Layout;
        
        Buffer m_LocalBuffer;
        MTL::Buffer* m_Buffer;
    };
}
#endif
