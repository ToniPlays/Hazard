#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Pipeline/Buffers.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalStorageBuffer : public StorageBuffer
    {
    public:
        MetalStorageBuffer(StorageBufferCreateInfo* createInfo) {};
        ~MetalStorageBuffer() {};
        
        void SetData(const BufferCopyRegion& copyRegion) override {};

        std::string& GetDebugName() override { return m_Name; }
        const size_t GetSize() const override { return m_Size; };
        
        //Metal specific
        MTL::Buffer* GetMetalBuffer() const { return m_Buffer; }
        
    private:
        void SetData_RT() {};
        
    private:
        std::string m_Name;
        size_t m_Size;
        
        MTL::Buffer* m_Buffer;
        Buffer m_LocalData;
    };
}
#endif
