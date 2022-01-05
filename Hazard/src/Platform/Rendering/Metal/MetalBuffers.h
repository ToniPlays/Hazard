#pragma once

#include "Hazard/Rendering/Pipeline/Buffers.h"

namespace Hazard::Rendering::Metal
{
    class MetalVertexBuffer : public VertexBuffer
    {
    public:
        MetalVertexBuffer(VertexBufferCreateInfo* createInfo) {};
        ~MetalVertexBuffer() {};

        void Bind(Ref<RenderCommandBuffer> cmdBuffer) override {};
        void Unbind(Ref<RenderCommandBuffer> cmdBuffer) override {};
        void SetData(const void* data, uint32_t size) override {};
        uint32_t GetSize() override { return m_Size; }

    private:
        BufferUsage m_Usage;
        uint32_t m_Size;
    };

    class MetalIndexBuffer : public IndexBuffer
    {
    public:
        MetalIndexBuffer(IndexBufferCreateInfo* createInfo) {};
        ~MetalIndexBuffer() {};

        void Bind(Ref<RenderCommandBuffer> cmdBuffer) override {};
        void Unbind(Ref<RenderCommandBuffer> cmdBuffer) override {};

        uint32_t GetCount() override { return m_Size; }

    private:
        void SetData(uint32_t* data, uint32_t size) override {};

        BufferUsage m_Usage;
        uint32_t m_Size;

    };
    class MetalUniformBuffer : public UniformBuffer
    {
    public:
        MetalUniformBuffer(UniformBufferCreateInfo* createInfo) {};
        ~MetalUniformBuffer() {};

        void Bind(Ref<RenderCommandBuffer> cmdBuffer) override {};
        void Unbind() override {};
        void SetData(const void* data) override {};
        const uint32_t GetBinding() const override { return m_Binding; };
        const uint32_t GetSize() const override { return m_Size; };

        uint32_t GetUsageFlags() override { return m_Usage; };

    private:

        uint32_t m_Size;
        uint32_t m_Binding;
        uint32_t m_Usage;
    };
}

