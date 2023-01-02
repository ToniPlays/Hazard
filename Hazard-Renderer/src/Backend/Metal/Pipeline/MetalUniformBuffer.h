#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Pipeline/Buffers.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalUniformBuffer : public UniformBuffer
    {
    public:
        MetalUniformBuffer(UniformBufferCreateInfo* createInfo);
        ~MetalUniformBuffer();
        
        void SetData(const BufferCopyRegion& copyRegion) override;

        std::string& GetName() override { return m_Name; }
        const uint32_t GetBinding() const override { return m_Binding; };
        const size_t GetSize() const override { return m_Size; };

        uint32_t GetUsageFlags() override { return m_Usage; };
        void AddUsageFlags(uint32_t flags) override { m_Usage |= flags; }
        
        //Metal specific
        MTL::Buffer* GetMetalBuffer() const { return m_Buffer; }
        
    private:
        void Invalidate_RT();
        
    private:
        std::string m_Name;
        uint32_t m_Binding;
        size_t m_Size;
        uint32_t m_Usage;
        
        MTL::Buffer* m_Buffer;
        Buffer m_LocalData;
    };
}

#endif
