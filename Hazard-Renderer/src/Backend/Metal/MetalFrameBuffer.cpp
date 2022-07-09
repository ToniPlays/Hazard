

#include "MetalFrameBuffer.h"
#ifdef HZR_INCLUDE_METAL
#include "MetalContext.h"
#include "MetalSwapchain.h"

#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    MetalFrameBuffer::MetalFrameBuffer(FrameBufferCreateInfo* info)
    {
        
        m_RenderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
        
        m_Specs.DebugName = info->DebugName;

        m_Specs.SwapChainTarget = info->SwapChainTarget;
        m_Specs.ClearOnLoad = info->ClearOnLoad;
        m_Specs.Samples = info->Samples;
        m_Specs.pFrameBuffer = info->pFrameBuffer;
        m_Specs.AttachmentCount = info->AttachmentCount;
        m_Specs.Attachments = info->Attachments;
        m_Specs.ClearColor = info->ClearColor;
        
        if (m_Specs.Width == 0) {
            m_Specs.Width = 1280;
            m_Specs.Height = 720;
        }
        else
        {
            m_Specs.Width = (uint32_t)info->Width;
            m_Specs.Height = (uint32_t)info->Height;
        }
        
        Resize(m_Specs.Width, m_Specs.Height, true);
    }
    MetalFrameBuffer::~MetalFrameBuffer() {
        m_RenderPassDescriptor->release();
    }
    void MetalFrameBuffer::Resize(uint32_t width, uint32_t height, bool force)
    {
        if (!force && (width == m_Specs.Width && height == m_Specs.Height))
            return;
        
        m_Specs.Width = width;
        m_Specs.Height = height;
        
        if(!m_Specs.SwapChainTarget) {
            Invalidate();
            return;
        }
        Invalidate();
        
    }
    void MetalFrameBuffer::Bind() {
        
    }
    void MetalFrameBuffer::Unbind() {
        
    }
    void MetalFrameBuffer::BindTexture(uint32_t attachmentIndex, uint32_t slot) const
    {
        
    }
    void MetalFrameBuffer::Invalidate()
    {
        
        auto swapchain = MetalContext::GetInstance()->GetSwapchain().As<MetalSwapchain>();

        MTL::ClearColor clearColor = { m_Specs.ClearColor.r, m_Specs.ClearColor.g, m_Specs.ClearColor.b, m_Specs.ClearColor.a };
        
        m_RenderPassDescriptor->setRenderTargetWidth(m_Specs.Width);
        m_RenderPassDescriptor->setRenderTargetHeight(m_Specs.Height);
        m_RenderPassDescriptor->setDefaultRasterSampleCount(1);
        
        MTL::RenderPassColorAttachmentDescriptor* colorAttachment = m_RenderPassDescriptor->colorAttachments()->object(0);
        
        colorAttachment->setClearColor(clearColor);
        colorAttachment->setLoadAction(MTL::LoadAction::LoadActionClear);
        colorAttachment->setStoreAction(MTL::StoreAction::StoreActionStore);
    }
}
#endif
