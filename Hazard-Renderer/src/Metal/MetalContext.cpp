#ifdef HZR_PLATFORM_MACOS

#include <GLFW/glfw3.h>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "MetalContext.h"
#include "MetalLayer.h"
#include "Platform/System/Window.h"

#include "MetalFrameBuffer.h"
#include "MetalRenderCommandBuffer.h"

namespace Hazard::Rendering::Metal
{
    MetalContext::MetalContext(WindowProps* props)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        s_Instance = this;
    }
    MetalContext::~MetalContext() {
        //m_Device->release();
    }
    void MetalContext::Init(Window *window, ApplicationCreateInfo *appInfo)
    {
        //m_Device = MTL::CreateSystemDefaultDevice();
        m_MetalLayer = new MetalLayer((GLFWwindow*)window->GetNativeWindow());
        m_CommandQueue = GetMetalDevice()->newCommandQueue();
    }
    void MetalContext::SetViewport(int x, int y, int w, int h) {
        
    }
    void MetalContext::BeginFrame()
    {
        
    }
    void MetalContext::Present()
    {
        
    }
    void MetalContext::BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass)
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
        buffer.As<MetalRenderCommandBuffer>()->EndRenderEncoder();
    }
    void MetalContext::SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth)
    {
        
    }
    void MetalContext::SetErrorListener(const ErrorCallback &callback) {
        
    }
    
    void MetalContext::Present(MTL::CommandBuffer* buffer) {
        s_Instance->m_MetalLayer->Present(buffer);
        
    }
    MTL::Device* MetalContext::GetMetalDevice()
    {
        return s_Instance->m_MetalLayer->GetDevice();
    }
}

#endif