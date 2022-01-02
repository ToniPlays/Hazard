
#include <hzrpch.h>

#include "MetalContext.h"
#include "Platform/System/Window.h"

namespace Hazard::Rendering::Metal
{
    MetalContext::MetalContext(WindowProps* props)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }
    MetalContext::~MetalContext() {
        
    }
    void MetalContext::Init(Window *window, ApplicationCreateInfo *appInfo) {
        m_Device = MTL::CreateSystemDefaultDevice();
    }
    void MetalContext::SetViewport(int x, int y, int w, int h) {
        
    }
    void MetalContext::SwapBuffers() {
        
    }
    void MetalContext::Begin() {
        
    }
    void MetalContext::End()
    {
        
    }
    void MetalContext::BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) {
    
    }
    void MetalContext::EndRenderPass(Ref<RenderCommandBuffer> buffer) {
        
    }
    void MetalContext::SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth) {
        
    }
    void MetalContext::SetErrorListener(const ErrorCallback &callback) {
        
    }
    DeviceSpec MetalContext::GetDeviceSpec() const {
        DeviceSpec spec = {};
        spec.Name = m_Device->name()->utf8String();
        return spec;
    }
}
