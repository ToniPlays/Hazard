#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL
#include "Core/GraphicsContext.h"
#include <Metal/Metal.hpp>

#include "MetalPhysicalDevice.h"

#include <GLFW/glfw3.h>

namespace HazardRenderer::Metal
{
    class MetalWindowLayer;

    class MetalContext : public GraphicsContext {
    public:
        MetalContext(WindowProps* props);
        ~MetalContext();

        void Init(Window* window, HazardRendererCreateInfo* info) override;
        void SetClearColor(const glm::vec4& color) override { m_ClearColor = color; }
        void SetViewport(int x, int y, int w, int h) override;
        void Present() override;
        void BeginFrame() override;
        //void Close() override {}
        
        //void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) override;
        //void EndRenderPass(Ref<RenderCommandBuffer> buffer) override;
        //void SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth) override;

        //void SetErrorListener(const ErrorCallback& callback) override;
        PhysicalDevice& GetDevice() override { return *m_PhysicalDevice; };

        static MTL::Device* GetMetalDevice() { return s_Instance->m_PhysicalDevice->GetMetalDevice(); };
        static MTL::CommandQueue* GetMetalCommandQueue() { return s_Instance->m_CommandQueue; }
        
    private:
        Window* m_Window;
        glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        static ErrorCallback s_Callback;
        
        inline static MetalContext* s_Instance;
        MetalPhysicalDevice* m_PhysicalDevice;
        
        MetalWindowLayer* m_MetalLayer;
        MTL::CommandQueue* m_CommandQueue;
    };
}
#endif