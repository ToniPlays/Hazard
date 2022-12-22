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
    };

    class MetalIndexBuffer : public IndexBuffer
    {
    public:
        MetalIndexBuffer(IndexBufferCreateInfo* createInfo);
        ~MetalIndexBuffer();
    };
    class MetalUniformBuffer : public UniformBuffer
    {
    public:
        MetalUniformBuffer(UniformBufferCreateInfo* createInfo);
        ~MetalUniformBuffer();
    };
}

#endif
