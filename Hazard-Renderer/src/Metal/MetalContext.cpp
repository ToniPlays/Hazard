
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
        m_PhysicalDevice = new MetalPhysicalDevice();
        m_MetalLayer = new MetalWindowLayer((GLFWwindow*)window->GetNativeWindow(), m_PhysicalDevice->GetMetalDevice());
    }
    void MetalContext::SetViewport(int x, int y, int w, int h) {
        
    }
    void MetalContext::BeginFrame()
    {
        
    }
    void MetalContext::Present()
    {
        MTL::Drawable* drawable = m_MetalLayer->GetNextDrawable();
        
        //MTL::RenderPassDescriptor* pass = MTL::RenderPassDescriptor->alloc
    }
    /*void MetalContext::BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass)
    {
        
        auto spec = renderPass->GetSpecs().TargetFrameBuffer->GetSpecification();
        auto fb = renderPass->GetSpecs().TargetFrameBuffer.As<MetalFrameBuffer>();
        
        MTL::ClearColor clearColor = { spec.ClearColor.r, spec.ClearColor.g, spec.ClearColor.b, spec.ClearColor.a };
        
        MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
        renderPassDescriptor->setRenderTargetWidth(1280);
        renderPassDescriptor->setRenderTargetHeight(720);
        renderPassDescriptor->setDefaultRasterSampleCount(1);
        
        MTL::RenderPassColorAttachmentDescriptor* colorAttachment = renderPassDescriptor->colorAttachments()->object(0);
        colorAttachment->setClearColor(clearColor);
        colorAttachment->setLoadAction(MTL::LoadAction::LoadActionClear);
        colorAttachment->setStoreAction(MTL::StoreAction::StoreActionStore);
        colorAttachment->setTexture(m_MetalLayer->GetDrawableTexture());
        
        buffer.As<MetalRenderCommandBuffer>()->BeginRenderEncoder(renderPassDescriptor);
         
    }
    void MetalContext::EndRenderPass(Ref<RenderCommandBuffer> buffer)
    {
        //buffer.As<MetalRenderCommandBuffer>()->EndRenderEncoder();
    }
    void MetalContext::SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth)
    {
        
    }
    void MetalContext::SetErrorListener(const ErrorCallback &callback) {
        
    }
    
    //void MetalContext::Present(MTL::CommandBuffer* buffer) {
    //    //s_Instance->m_MetalLayer->Present(buffer);
    //}
     */
}

#endif
