#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Pipeline/Buffers.h"

#include <Metal/Metal.hpp>

namespace HazardRenderer::Metal
{
    class MetalVertexBuffer : public VertexBuffer
    {
    public:
        MetalVertexBuffer(VertexBufferCreateInfo* createInfo);
        ~MetalVertexBuffer();

        void Bind(Ref<RenderCommandBuffer> cmdBuffer, uint32_t binding = 0) override;
        void Unbind(Ref<RenderCommandBuffer> cmdBuffer) override;
        void SetData(const void* data, uint32_t size) override;
        uint32_t GetSize() override { return m_Size; }
        
        const BufferLayout& GetLayout() const override { return m_Layout; }
        
        std::string& GetDebugName() override { return m_DebugName; }
        

    private:
        BufferUsage m_Usage;
        std::string m_DebugName;
        uint32_t m_Size = 0;
        BufferLayout m_Layout;
        
        MTL::Buffer* m_Buffer;
    };

    class MetalIndexBuffer : public IndexBuffer
    {
    public:
        MetalIndexBuffer(IndexBufferCreateInfo* createInfo);
        ~MetalIndexBuffer();

        void Bind(Ref<RenderCommandBuffer> cmdBuffer) override;
        void Unbind(Ref<RenderCommandBuffer> cmdBuffer) override {};

        uint32_t GetCount() override { return m_Size; }
        std::string& GetDebugName() override { return m_DebugName; }

    private:
        void SetData(uint32_t* data, uint32_t size) override;

        BufferUsage m_Usage;
        uint32_t m_Size = 0;
        std::string m_DebugName;
        
        MTL::Buffer* m_Buffer;

    };
    class MetalUniformBuffer : public UniformBuffer
    {
    public:
        MetalUniformBuffer(UniformBufferCreateInfo* createInfo);
        ~MetalUniformBuffer();

        void Bind(Ref<RenderCommandBuffer> cmdBuffer) override;
        void Unbind() override;
        void SetData(const void* data, uint32_t size) override;
        const uint32_t GetBinding() const override { return m_Binding; };
        const uint32_t GetSize() const override { return m_Size; };

        uint32_t GetUsageFlags() override { return m_Usage; };
        
        std::string& GetName() override { return m_Name; }

    private:

        uint32_t m_Size = 0;
        uint32_t m_Binding = 0;
        uint32_t m_Usage;
        
        std::string m_Name;
        
        MTL::Buffer* m_Buffer;
    };
}

#endif
