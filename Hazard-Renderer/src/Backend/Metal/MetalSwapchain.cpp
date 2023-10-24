
#include "MetalSwapchain.h"

#ifdef HZR_INCLUDE_METAL

#include "MetalRenderCommandBuffer.h"
#include "MetalWindowLayer.h"
#include "Backend/Core/RenderPass.h"


namespace HazardRenderer::Metal {
    
    MetalSwapchain::MetalSwapchain(Window* window)
    {
        m_Window = window;
    }

    void MetalSwapchain::Create(uint32_t* width, uint32_t* height, bool vsync)
    {
        glm::vec2 scale = m_Window->GetWindowInfo().FramebufferScale;
        
        m_Width = *width * scale.x;
        m_Height = *height * scale.y;
        
        m_RenderCommandBuffer = RenderCommandBuffer::CreateFromSwapchain("Swapchain");
        
        if (m_DefaultFramebuffer)
        {
            m_DefaultFramebuffer->Resize_RT(m_Width, m_Height);
            return;
        }

        if (nullptr == nullptr) //TODO: Fix this
        {
            //Create default target

            FrameBufferCreateInfo frameBufferInfo = {};
            frameBufferInfo.DebugName = "ScreenFBO";
            frameBufferInfo.SwapChainTarget = true;
            frameBufferInfo.AttachmentCount = 1;
            frameBufferInfo.Attachments = { { ImageFormat::RGBA } };

            m_DefaultFramebuffer = FrameBuffer::Create(&frameBufferInfo);

            RenderPassCreateInfo renderPassInfo = {};
            renderPassInfo.DebugName = "ScreenTarget";
            renderPassInfo.pTargetFrameBuffer = m_DefaultFramebuffer;

            m_DefaultRenderPass = RenderPass::Create(&renderPassInfo);
        }
        else
        {
            m_DefaultFramebuffer = FrameBuffer::Create(nullptr);

            RenderPassCreateInfo renderPassInfo = {};
            renderPassInfo.DebugName = "ScreenTarget";
            renderPassInfo.pTargetFrameBuffer = m_DefaultFramebuffer;

            m_DefaultRenderPass = RenderPass::Create(&renderPassInfo);
        }
        Resize(m_Width, m_Height);
    }

    void MetalSwapchain::Resize(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;
        
        MetalContext::GetWindowLayer()->Resize(m_Width, m_Height);
    }

    void MetalSwapchain::BeginFrame()
    {
        m_Drawable = MetalContext::GetWindowLayer()->GetNextDrawable();
        m_RenderCommandBuffer->Begin();
    }
    void MetalSwapchain::Present()
    {
        m_RenderCommandBuffer->End();

        auto cmdBuffer = m_RenderCommandBuffer.As<MetalRenderCommandBuffer>();
        auto mtlCommandBuffer = cmdBuffer->GetMetalCommandBuffer();
        
        mtlCommandBuffer->presentDrawable(m_Drawable);
        mtlCommandBuffer->commit();
        mtlCommandBuffer->waitUntilCompleted();
    }
}
#endif
