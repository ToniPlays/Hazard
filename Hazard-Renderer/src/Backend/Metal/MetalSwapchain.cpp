
#include "MetalSwapchain.h"
#include "MetalRenderCommandBuffer.h"
#include "MetalWindowLayer.h"

#include <Metal/Metal.hpp>
#include <Foundation/Foundation.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal {
    
    MetalSwapchain::MetalSwapchain(MetalContext* context, FrameBufferCreateInfo* targetInfo)
    {
        if (targetInfo == nullptr) {
            //Create default target

            FrameBufferCreateInfo frameBufferInfo = {};
            frameBufferInfo.DebugName = "ScreenFBO";
            frameBufferInfo.SwapChainTarget = true;
            frameBufferInfo.AttachmentCount = 2;
            frameBufferInfo.Attachments = { { ImageFormat::RGBA }, { ImageFormat::Depth } };
            
            m_FrameBuffer = FrameBuffer::Create(&frameBufferInfo);

            RenderPassCreateInfo renderPassInfo = {};
            renderPassInfo.DebugName = "ScreenTarget";
            renderPassInfo.pTargetFrameBuffer = m_FrameBuffer;

            m_RenderPass = RenderPass::Create(&renderPassInfo);
        }
        else
        {
            m_FrameBuffer = FrameBuffer::Create(targetInfo);

            RenderPassCreateInfo renderPassInfo = {};
            renderPassInfo.DebugName = "ScreenTarget";
            renderPassInfo.pTargetFrameBuffer = m_FrameBuffer;
            
            m_RenderPass = RenderPass::Create(&renderPassInfo);
        }
        
        m_RenderCommandBuffer = RenderCommandBuffer::CreateFromSwapchain("Swapchain");
    }
    void MetalSwapchain::BeginFrame()
    {
        m_Drawable = MetalContext::GetWindowLayer()->GetNextDrawable();
        m_RenderCommandBuffer->Begin();
    }
    void MetalSwapchain::Present()
    {
        m_RenderCommandBuffer->End();
        
        MTL::CommandBuffer* cmdBuffer = m_RenderCommandBuffer.As<MetalRenderCommandBuffer>()->GetMetalCommandBuffer();
        
        cmdBuffer->presentDrawable(m_Drawable);
        cmdBuffer->commit();
        cmdBuffer->waitUntilCompleted();
        cmdBuffer->release();
        m_Drawable->release();
        
    }
}
