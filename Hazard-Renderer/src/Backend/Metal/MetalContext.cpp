
#include "MetalContext.h"
#ifdef HZR_INCLUDE_METAL
#include <GLFW/glfw3.h>


//#include <Metal/Metal.hpp>
//#include <QuartzCore/QuartzCore.hpp>

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
        /*
        m_PhysicalDevice = hnew MetalPhysicalDevice();
        m_MetalLayer = hnew MetalWindowLayer(*window, m_PhysicalDevice->GetMetalDevice());
        
        m_Swapchain = Ref<MetalSwapchain>::Create(this, info->pTargetFrameBuffer);
        m_Swapchain->Create(m_MetalLayer->GetWidth(), m_MetalLayer->GetHeight(), info->VSync);
         */
        
    }

    void MetalContext::BeginFrame()
    {
        m_Swapchain->BeginFrame();
    }
    void MetalContext::Present()
    {
        
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
