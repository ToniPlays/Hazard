
#include "MetalContext.h"
#ifdef HZR_INCLUDE_METAL
#include <GLFW/glfw3.h>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "MetalWindowLayer.h"
#include "Core/Window.h"

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
    }
    void MetalContext::SetViewport(int x, int y, int w, int h)
    {
        m_MetalLayer->Resize(w, h);
    }
    void MetalContext::BeginFrame()
    {
        m_Drawable = m_MetalLayer->GetNextDrawable();
    }
    void MetalContext::Present()
    {
        m_SwapchainCommandBuffer->GetMetalCommandBuffer()->presentDrawable(m_Drawable);
        m_SwapchainCommandBuffer->GetMetalCommandBuffer()->commit();
    }
    void MetalContext::BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass)
    {
        auto spec = renderPass->GetSpecs().TargetFrameBuffer->GetSpecification();
        auto fb = renderPass->GetSpecs().TargetFrameBuffer.As<MetalFrameBuffer>();
        
        
        MTL::ClearColor clearColor = MTL::ClearColor::Make(spec.ClearColor.r, spec.ClearColor.g, spec.ClearColor.b, spec.ClearColor.a);
        
        MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::renderPassDescriptor();
        renderPassDescriptor->setRenderTargetWidth(fb->GetWidth());
        renderPassDescriptor->setRenderTargetHeight(fb->GetHeight());
        
        MTL::RenderPassColorAttachmentDescriptorArray* attachemts = renderPassDescriptor->colorAttachments();
        attachemts->object(0)->setClearColor(clearColor);
        attachemts->object(0)->setLoadAction(MTL::LoadActionClear);
        attachemts->object(0)->setStoreAction(MTL::StoreActionStore);
        
        if(fb->GetSpecification().SwapChainTarget) {
            attachemts->object(0)->setTexture(m_Drawable->texture());
        }
        
        buffer.As<MetalRenderCommandBuffer>()->BeginRenderEncoder(renderPassDescriptor);
    }
    void MetalContext::EndRenderPass(Ref<RenderCommandBuffer> buffer)
    {
        Ref<MetalRenderCommandBuffer> cmdBuffer = buffer.As<MetalRenderCommandBuffer>();
        cmdBuffer->EndRenderEncoder();
    }
    /*
    void MetalContext::SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth)
    {
        
    }
    void MetalContext::SetErrorListener(const ErrorCallback &callback) {
        
    }
    */
}

#endif
