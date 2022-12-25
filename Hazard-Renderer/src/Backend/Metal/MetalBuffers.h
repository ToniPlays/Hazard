#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Pipeline/Buffers.h"

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
        size_t GetSize() const override { return m_Size; }
        std::string& GetDebugName() override { return m_DebugName; };
        const BufferLayout& GetLayout() const override { return m_Layout; }

        //Metal specific
        MTL::Buffer* GetMetalBuffer() const { return m_Buffer; }
        
    private:
        size_t m_Size;
        std::string m_DebugName;
        BufferLayout m_Layout;
        BufferUsage m_Usage;
        
        Buffer m_LocalBuffer;
        
        MTL::Buffer* m_Buffer;
    };

    class MetalIndexBuffer : public IndexBuffer
    {
    public:
        MetalIndexBuffer(IndexBufferCreateInfo* createInfo);
        ~MetalIndexBuffer();
        
        size_t GetCount() const override { return m_Size / sizeof(uint32_t); }
        std::string& GetDebugName() override { return m_DebugName; };
        
        //Metal specific
        MTL::Buffer* GetMetalBuffer() const { return m_Buffer; }

    private:
        void SetData(const BufferCopyRegion& copyRegion) override;
        void SetData_RT(const BufferCopyRegion& copyRegion);
        
    private:
        size_t m_Size;
        std::string m_DebugName;
        Buffer m_LocalBuffer;
        
        MTL::Buffer* m_Buffer;
        
        
    };
    class MetalUniformBuffer : public UniformBuffer
    {
    public:
        MetalUniformBuffer(UniformBufferCreateInfo* createInfo) {};
        ~MetalUniformBuffer() {};
        
        void SetData(const BufferCopyRegion& copyRegion) override {};

        std::string& GetName() override { return m_Name; }
        const uint32_t GetBinding() const override { return m_Binding; };
        const size_t GetSize() const override { return m_Size; };

        uint32_t GetUsageFlags() override { return m_Usage; };
    private:
        std::string m_Name;
        uint32_t m_Binding;
        size_t m_Size;
        uint32_t m_Usage;
        
        MTL::Buffer* m_Buffer;
    };
}

#endif
