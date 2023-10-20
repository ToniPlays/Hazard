#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/GPUBuffer.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalGPUBuffer : public GPUBuffer
    {
    public:
        MetalGPUBuffer(BufferCreateInfo* createInfo);
        ~MetalGPUBuffer();
        
        const std::string& GetDebugName() const override { return m_DebugName; };
        
        const uint64_t GetSize() const override { return m_Size; }
        Buffer ReadData(const BufferCopyRegion& region) override { return Buffer(); }
        
        void SetData(const BufferCopyRegion& copyRegion) override;
        const uint32_t GetUsageFlags() const override { return m_UsageFlags; }
 
        
        //Metal specific
        MTL::Buffer* GetMetalBuffer() const { return m_Buffer; }
        
    private:
        void SetData_RT(const BufferCopyRegion& copyRegion);
        
    private:
        std::string m_DebugName;
        uint64_t m_Size;
        uint32_t m_UsageFlags;
        
        Buffer m_LocalBuffer;
        MTL::Buffer* m_Buffer;
    };
}
#endif
