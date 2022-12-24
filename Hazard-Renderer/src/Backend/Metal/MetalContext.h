#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalPhysicalDevice.h"
#include <GLFW/glfw3.h>

namespace HazardRenderer::Metal
{
    class MetalWindowLayer;
    class MetalSwapchain;

    class MetalContext : public GraphicsContext
    {
    public:
        MetalContext(WindowProps* props);
        ~MetalContext();
        
        void Init(Window* window, HazardRendererCreateInfo* info) override;
        void BeginFrame() override;
        void Present() override;
        
        void SetClearColor(const glm::vec4& color) override;
        
        Ref<PhysicalDevice> GetDevice() override { return m_PhysicalDevice; };
        Ref<Swapchain> GetSwapchain() override { return m_Swapchain; };
        DefaultResources& GetDefaultResources() override { return m_Resources; };
            
        //Metal specific
        static MetalContext* GetInstance() { return s_Instance; }
        static MetalWindowLayer* GetWindowLayer() { return s_Instance->m_MetalLayer; }
        
    private:
        Window* m_Window;
        glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        static ErrorCallback s_Callback;
        
        inline static MetalContext* s_Instance;
        
        MetalWindowLayer* m_MetalLayer;
        Ref<MetalPhysicalDevice> m_PhysicalDevice;
        Ref<MetalSwapchain> m_Swapchain;
    
        DefaultResources m_Resources;
    };
}
#endif
