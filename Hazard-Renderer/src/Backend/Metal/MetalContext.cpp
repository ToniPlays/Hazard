
#include "MetalContext.h"
#ifdef HZR_INCLUDE_METAL
#include <GLFW/glfw3.h>


#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "MetalWindowLayer.h"
#include "Backend/Core/Window.h"

#include "MetalSwapchain.h"
#include "MetalFrameBuffer.h"
#include "MetalRenderCommandBuffer.h"

namespace HazardRenderer::Metal
{
    MetalContext::MetalContext(WindowProps* props)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        s_Instance = this;
    }
    MetalContext::~MetalContext() {
        //m_Device->release();
    }
    void MetalContext::Init(Window* window, HazardRendererCreateInfo* info)
    {
        m_Window = window;
        m_PhysicalDevice = new MetalPhysicalDevice();
        m_MetalLayer = new MetalWindowLayer((GLFWwindow*)window->GetNativeWindow(), m_PhysicalDevice->GetMetalDevice());
        
        m_Swapchain = Ref<MetalSwapchain>::Create(this, info->pTargetFrameBuffer);
        m_Swapchain->Create(m_Window->GetWidth(), m_Window->GetHeight(), info->VSync);
    }
    void MetalContext::SetViewport(int x, int y, int w, int h)
    {
        m_MetalLayer->Resize(w, h);
    }
    void MetalContext::BeginFrame()
    {
        m_Swapchain->BeginFrame();
        BeginRenderPass(m_Swapchain->GetSwapchainBuffer(), m_Swapchain->GetRenderPass());
    }
    void MetalContext::Present()
    {
        EndRenderPass(m_Swapchain->GetSwapchainBuffer());
        m_Swapchain->Present();
    }
    void MetalContext::BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass)
    {
        
        auto spec = renderPass->GetSpecs().TargetFrameBuffer->GetSpecification();
        auto fb = renderPass->GetSpecs().TargetFrameBuffer.As<MetalFrameBuffer>();
        
        MTL::ClearColor clearColor;
        
        if(spec.SwapChainTarget) {
            clearColor = { m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a };
        }
        else {
            clearColor = { spec.ClearColor.r, spec.ClearColor.g, spec.ClearColor.b, spec.ClearColor.a };
        }
        
        MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
        renderPassDescriptor->autorelease();
        renderPassDescriptor->setRenderTargetWidth(fb->GetWidth());
        renderPassDescriptor->setRenderTargetHeight(fb->GetHeight());
        renderPassDescriptor->setDefaultRasterSampleCount(1);
        
        MTL::RenderPassColorAttachmentDescriptor* colorAttachment = renderPassDescriptor->colorAttachments()->object(0);
        
        colorAttachment->setClearColor(clearColor);
        colorAttachment->setLoadAction(MTL::LoadAction::LoadActionClear);
        colorAttachment->setStoreAction(MTL::StoreAction::StoreActionStore);
        colorAttachment->setTexture(m_Swapchain->GetDrawable()->texture());
        
        buffer.As<MetalRenderCommandBuffer>()->BeginRenderEncoder(renderPassDescriptor);
        renderPassDescriptor->release();        
    }
    void MetalContext::EndRenderPass(Ref<RenderCommandBuffer> buffer)
    {
        buffer.As<MetalRenderCommandBuffer>()->EndRenderEncoder();
    }
     /*
    void MetalContext::SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth)
    {
        
    }
    void MetalContext::SetErrorListener(const ErrorCallback &callback) {
        
    }*/
    
    //void MetalContext::Present(MTL::CommandBuffer* buffer) {
    //    s_Instance->m_MetalLayer->Present(buffer);
    //}
}

#endif
