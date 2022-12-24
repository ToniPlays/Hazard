#pragma once

#include "Backend/Core/Swapchain.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalContext.h"

namespace CA
{
    class MetalDrawable;
}

namespace HazardRenderer::Metal {
    
    class MetalSwapchain : public Swapchain {
    public:
        
        MetalSwapchain(MetalContext* context, FrameBufferCreateInfo* targetInfo);
        ~MetalSwapchain() = default;
        
        void Resize(uint32_t width, uint32_t height) override;

        void BeginFrame() override;
        void Present() override;

        uint32_t GetWidth() const override { return m_Width; };
        uint32_t GetHeight() const override { return m_Height; };

        Ref<RenderCommandBuffer> GetSwapchainBuffer() override { return m_RenderCommandBuffer; }

        Ref<RenderPass> GetRenderPass() override { return m_RenderPass; };
        Ref<FrameBuffer> GetRenderTarget() override { return m_FrameBuffer; };
        
        //Metal specific
        CA::MetalDrawable* GetDrawable() { return m_Drawable; }
        void Create(uint32_t* width, uint32_t* height, bool vsync = true);
        
    private:
        uint32_t m_Width;
        uint32_t m_Height;
        
        Ref<RenderPass> m_RenderPass;
        Ref<RenderCommandBuffer> m_RenderCommandBuffer;
        Ref<FrameBuffer> m_FrameBuffer;
        
        CA::MetalDrawable* m_Drawable;
    };
}
#endif
