#pragma once

#include "Core/Core.h"
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
        const DefaultResources& GetDefaultResources() const override { return m_DefaultResources; };
            
        //Metal specific
        static Ref<MetalPhysicalDevice> GetMetalDevice() { return s_Instance->m_PhysicalDevice; }
        static MetalContext* GetInstance() { return s_Instance; }
        static MetalWindowLayer* GetWindowLayer();
    
    private:
        Window* m_Window;

        static ErrorCallback s_Callback;
        
        inline static MetalContext* s_Instance;
        Ref<MetalPhysicalDevice> m_PhysicalDevice;
        Ref<MetalSwapchain> m_Swapchain;
    
        DefaultResources m_DefaultResources;
    };
}
#endif
