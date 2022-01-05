
#include <hzrpch.h>

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
        m_Device = MTL::CreateSystemDefaultDevice();
        m_MetalLayer = new MetalLayer();
        
        m_CommandQueue = m_Device->newCommandQueue();
    }
    void MetalContext::SetViewport(int x, int y, int w, int h) {
        
    }
    void MetalContext::SwapBuffers()
    {
        
    }
    void MetalContext::Begin()
    {
        
    }
    void MetalContext::End()
    {
        
    }
    void MetalContext::BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass)
    {
        
        auto spec = renderPass->GetSpecs().TargetFrameBuffer->GetSpecification();
        auto fb = renderPass->GetSpecs().TargetFrameBuffer.As<MetalFrameBuffer>();
        
        MTL::ClearColor clearColor = { spec.ClearColor.r, spec.ClearColor.g, spec.ClearColor.b, spec.ClearColor.a };
        
        MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
        renderPassDescriptor->setRenderTargetWidth(spec.Width);
        renderPassDescriptor->setRenderTargetHeight(spec.Height);
        renderPassDescriptor->setDefaultRasterSampleCount(1);
        
        MTL::RenderPassColorAttachmentDescriptor* colorAttachment = renderPassDescriptor->colorAttachments()->object(0);
        colorAttachment->setClearColor(clearColor);
        
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
    DeviceSpec MetalContext::GetDeviceSpec() const
    {
        DeviceSpec spec = {};
        spec.Name = m_Device->name()->utf8String();
        return spec;
    }
}
