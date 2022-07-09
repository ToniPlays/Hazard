#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "MetalPhysicalDevice.h"

#include <GLFW/glfw3.h>

namespace HazardRenderer::Metal
{
    class MetalWindowLayer;
    class MetalSwapchain;

    class MetalContext : public GraphicsContext {
    public:
        MetalContext(WindowProps* props);
        ~MetalContext();

        void Init(Window* window, HazardRendererCreateInfo* info) override;
        void SetClearColor(const glm::vec4& color) override { m_ClearColor = color; }
        void SetViewport(int x, int y, int w, int h) override;
        void Present() override;
        void BeginFrame() override;
        
        void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) override;
        void BeginRenderPass_RT(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass);
        void EndRenderPass(Ref<RenderCommandBuffer> buffer) override;
        void EndRenderPass_RT(Ref<RenderCommandBuffer> buffer);
        //void SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth) override;

        //void SetErrorListener(const ErrorCallback& callback) override;
        PhysicalDevice& GetDevice() override { return *m_PhysicalDevice; };
        Ref<Swapchain> GetSwapchain() override { return m_Swapchain; };
        
        static MetalContext* GetInstance() { return s_Instance; }
        static MTL::Device* GetMetalDevice() { return s_Instance->m_PhysicalDevice->GetMetalDevice(); };
        static MTL::CommandQueue* GetMetalCommandQueue() { return s_Instance->m_PhysicalDevice->GetMetalCommandQueue(); }
        static MetalWindowLayer* GetWindowLayer() { return s_Instance->m_MetalLayer; }
        
    private:
        Window* m_Window;
        glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        static ErrorCallback s_Callback;
        
        inline static MetalContext* s_Instance;
        MetalPhysicalDevice* m_PhysicalDevice;
        
        MetalWindowLayer* m_MetalLayer;
        Ref<MetalSwapchain> m_Swapchain;
    };
}
#endif
