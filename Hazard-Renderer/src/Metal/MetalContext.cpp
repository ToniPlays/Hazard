
#include "MetalContext.h"
#ifdef HZR_INCLUDE_METAL
#include <GLFW/glfw3.h>

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
        Renderer::Init(this);
        s_Instance = this;
    }
    MetalContext::~MetalContext()
    {
        
    }
    void MetalContext::Init(Window* window, HazardRendererCreateInfo* info)
    {
        m_Window = window;
        
        uint32_t w = window->GetWidth();
        uint32_t h = window->GetHeight();
        
        m_PhysicalDevice = hnew MetalPhysicalDevice();
        m_MetalLayer = hnew MetalWindowLayer(*window, m_PhysicalDevice->GetMetalDevice());
        m_MetalLayer->Resize(w, h);
        
        m_Swapchain = Ref<MetalSwapchain>::Create(window);
        m_Swapchain->Create(&w, &h, m_Window->IsVSync());
        
        uint32_t data = 0xFFFFFFFF;
        Image2DCreateInfo whiteTexture = {};
        whiteTexture.DebugName = "DefaultWhiteTexture";
        whiteTexture.Extent.Width = 1;
        whiteTexture.Extent.Height = 1;
        whiteTexture.Mips = 1;
        whiteTexture.Data = Buffer(&data, sizeof(uint32_t));
        whiteTexture.Format = ImageFormat::RGBA;
        whiteTexture.Usage = ImageUsage::Texture;

        m_DefaultResources.WhiteTexture = Image2D::Create(&whiteTexture);
    }

    void MetalContext::BeginFrame()
    {
        m_Swapchain->BeginFrame();
    }
    void MetalContext::Present()
    {
        m_Swapchain->Present();
    }
    void MetalContext::SetClearColor(const glm::vec4 &color)
    {
        m_Swapchain->GetRenderTarget()->GetSpecification().ClearColor = color;
    }
}

#endif
