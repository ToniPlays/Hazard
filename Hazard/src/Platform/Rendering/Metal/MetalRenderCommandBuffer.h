#pragma once

#include "Hazard/Rendering/RenderCommandBuffer.h"

namespace Hazard::Rendering::Metal {
    class MetalRenderCommandBuffer : public RenderCommandBuffer {
    public:
        MetalRenderCommandBuffer(uint32_t size, const std::string& name) {};
        MetalRenderCommandBuffer(const std::string& name, bool swapchain) {};
        ~MetalRenderCommandBuffer() = default;

        virtual uint32_t GetFrameIndex() { return 0; };

        void Begin() {};
        void End() {};
        void Submit() {};
    };
}
