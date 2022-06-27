#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/RenderCommandBuffer.h"

#include <Metal/Metal.hpp>

namespace HazardRenderer::Metal {
    class MetalRenderCommandBuffer : public RenderCommandBuffer {
    public:
        MetalRenderCommandBuffer(uint32_t size, const std::string& name);
        MetalRenderCommandBuffer(const std::string& name, bool swapchain);
        ~MetalRenderCommandBuffer() = default;

        virtual uint32_t GetFrameIndex() override { return 0; };
        virtual bool IsRecording() override { return false; }
        MTL::CommandBuffer* GetMetalCommandBuffer() { return m_CommandBuffer; }
        MTL::RenderCommandEncoder* GetEncoder() { return m_Encoder; }
        
        void Begin() override;
        void End() override;
        void Submit() override;
        
        void BeginRenderEncoder(MTL::RenderPassDescriptor* descriptor);
        void EndRenderEncoder() { m_Encoder->endEncoding(); }
        
    private:
        bool m_OwnedBySwapchain;
        MTL::CommandBuffer* m_CommandBuffer;
        MTL::RenderCommandEncoder* m_Encoder;
    };
}
#endif
