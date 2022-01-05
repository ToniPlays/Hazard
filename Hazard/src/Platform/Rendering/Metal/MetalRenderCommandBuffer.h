#pragma once

#include "Hazard/Rendering/RenderCommandBuffer.h"


#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace Hazard::Rendering::Metal {
    class MetalRenderCommandBuffer : public RenderCommandBuffer {
    public:
        MetalRenderCommandBuffer(uint32_t size, const std::string& name);
        MetalRenderCommandBuffer(const std::string& name, bool swapchain);
        ~MetalRenderCommandBuffer() = default;

        virtual uint32_t GetFrameIndex() { return 0; };
        MTL::CommandBuffer* GetMetalCommandBuffer() { return m_CommandBuffer; }
        MTL::RenderCommandEncoder* GetEncoder() { return m_Encoder; }
        
        void Begin();
        void End();
        void Submit();
        
        void BeginRenderEncoder(MTL::RenderPassDescriptor* descriptor);
        void EndRenderEncoder() { m_Encoder->endEncoding(); }
        
    private:
        bool m_OwnedBySwapchain;
        MTL::CommandBuffer* m_CommandBuffer;
        MTL::RenderCommandEncoder* m_Encoder;
    };
}
