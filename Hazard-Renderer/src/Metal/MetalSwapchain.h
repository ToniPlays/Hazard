#pragma once

#include "Core/Rendering/Swapchain.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalContext.h"
#include "MetalWindowLayer.h"

namespace CA
{
    class MetalDrawable;
}

namespace HazardRenderer::Metal {
    
    class MetalSwapchain : public Swapchain {
    public:
        
        MetalSwapchain(Window* window);
        ~MetalSwapchain() = default;
        
        void Resize(uint32_t width, uint32_t height) override;

        void BeginFrame() override;
        void Present() override;

        uint32_t GetWidth() const override { return m_MetalLayer->GetWidth(); };
        uint32_t GetHeight() const override { return m_MetalLayer->GetHeight(); };

        Ref<RenderCommandBuffer> GetSwapchainBuffer() override { return m_RenderCommandBuffer; }

        Ref<RenderPass> GetRenderPass() override { return m_DefaultRenderPass; };
        Ref<FrameBuffer> GetRenderTarget() override { return m_DefaultFramebuffer; };
        
        //Metal specific
        CA::MetalDrawable* GetDrawable() { return m_Drawable; }
        MetalWindowLayer* GetWindowLayer() { return m_MetalLayer; };
        
        void Create(uint32_t* width, uint32_t* height, bool vsync = true);
        
    private:
        Window* m_Window = nullptr;
        MetalWindowLayer* m_MetalLayer;
        
        Ref<RenderPass> m_RenderPass;
        Ref<RenderCommandBuffer> m_RenderCommandBuffer;
        Ref<FrameBuffer> m_DefaultFramebuffer;
        Ref<RenderPass> m_DefaultRenderPass;
        
        CA::MetalDrawable* m_Drawable;
    };
}
#endif
