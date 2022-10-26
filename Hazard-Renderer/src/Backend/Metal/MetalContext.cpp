
#include "MetalContext.h"
#ifdef HZR_INCLUDE_METAL
#include <GLFW/glfw3.h>


#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "MetalWindowLayer.h"
#include "Backend/Core/Window.h"
#include "Backend/Core/Renderer.h"

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
        m_PhysicalDevice = hnew MetalPhysicalDevice();
        m_MetalLayer = hnew MetalWindowLayer(*window, m_PhysicalDevice->GetMetalDevice());
        
        m_Swapchain = Ref<MetalSwapchain>::Create(this, info->pTargetFrameBuffer);
        m_Swapchain->Create(m_MetalLayer->GetWidth(), m_MetalLayer->GetHeight(), info->VSync);
        
    }
    void MetalContext::SetViewport(int x, int y, int w, int h)
    {
        m_MetalLayer->Resize(w, h);
        m_Swapchain->Resize(w, h);
    }
    void MetalContext::BeginFrame()
    {
        m_Swapchain->BeginFrame();
    }
    void MetalContext::Present()
    {
        MetalContext* instance = this;
        Renderer::Submit([instance]() mutable {
            instance->m_Swapchain->Present();
            });
    }
    void MetalContext::BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) {
        MetalContext* instance = this;
        Renderer::Submit([instance, buffer, renderPass]() mutable {
            instance->BeginRenderPass_RT(buffer, renderPass);
            });
    }
    void MetalContext::BeginRenderPass_RT(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass)
    {
        
        auto spec = renderPass->GetSpecs().TargetFrameBuffer->GetSpecification();
        auto fb = renderPass->GetSpecs().TargetFrameBuffer.As<MetalFrameBuffer>();
        
        MTL::RenderPassDescriptor* descriptor = fb->GetMetalRenderPassDescriptor();
        if(spec.SwapChainTarget)
        {
            descriptor->colorAttachments()->object(0)->setTexture(m_Swapchain->GetDrawable()->texture());
        }
        
        buffer.As<MetalRenderCommandBuffer>()->BeginRenderEncoder(descriptor);
    }
    void MetalContext::EndRenderPass(Ref<RenderCommandBuffer> buffer) {
        MetalContext* instance = this;
        Renderer::Submit([instance, buffer]() mutable {
            instance->EndRenderPass_RT(buffer);
            });
    }

    void MetalContext::EndRenderPass_RT(Ref<RenderCommandBuffer> buffer)
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
