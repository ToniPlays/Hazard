#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Buffers.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalArgumentBuffer : public ArgumentBuffer
    {
    public:
        MetalArgumentBuffer(ArgumentBufferCreateInfo* createInfo);
        ~MetalArgumentBuffer();
        
        void SetData(const BufferCopyRegion& copyRegion) override;

        std::string& GetDebugName() override { return m_DebugName; }
        const size_t GetSize() const override { return m_Size; };
        
        //Metal specific
        MTL::Buffer* GetMetalBuffer() const { return m_Buffer; }
        
    private:
        void SetData_RT() {};
        
    private:
        std::string m_DebugName;
        size_t m_Size;
        
        MTL::Buffer* m_Buffer;
        Buffer m_LocalBuffer;
    };
}
#endif
