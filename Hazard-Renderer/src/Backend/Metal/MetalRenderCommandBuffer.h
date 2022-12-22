#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/RenderCommandBuffer.h"

#include <Metal/Metal.hpp>

namespace HazardRenderer::Metal
{
    class MetalRenderCommandBuffer : public RenderCommandBuffer {
    public:
        MetalRenderCommandBuffer(uint32_t size, const std::string& name);
        MetalRenderCommandBuffer(const std::string& name, bool swapchain);
        ~MetalRenderCommandBuffer() = default;
    };
}
#endif
