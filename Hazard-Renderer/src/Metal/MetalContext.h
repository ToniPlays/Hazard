#pragma once

#include "Core/Core.h"

#ifdef HZR_INCLUDE_METAL
#include "Core/GraphicsContext.h"
#include <Metal/Metal.hpp>

#include "MetalPhysicalDevice.h"
#include "MetalRenderCommandBuffer.h"

#include <GLFW/glfw3.h>

namespace CA {
class MetalDrawable;
}

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
        
        void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) override;
        void EndRenderPass(Ref<RenderCommandBuffer> buffer) override;
        //void SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth) override;

        //void SetErrorListener(const ErrorCallback& callback) override;
        PhysicalDevice& GetDevice() override { return *m_PhysicalDevice; };

        static MTL::Device* GetMetalDevice() { return s_Instance->m_PhysicalDevice->GetMetalDevice(); };
        static MTL::CommandQueue* GetMetalCommandQueue() {
            return s_Instance->m_PhysicalDevice->GetMetalCommandQueue();
        }
        static void SetSwpachainCommandBuffer(Ref<MetalRenderCommandBuffer> cmdBuffer) {
            s_Instance->m_SwapchainCommandBuffer = cmdBuffer;
        }
        static glm::vec4 GetClearColor() { return s_Instance->m_ClearColor; }
        
    private:
        Window* m_Window;
        glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        static ErrorCallback s_Callback;
        
        inline static MetalContext* s_Instance;
        
        MetalPhysicalDevice* m_PhysicalDevice;
        Ref<MetalRenderCommandBuffer> m_SwapchainCommandBuffer;
        MetalWindowLayer* m_MetalLayer;
        CA::MetalDrawable* m_Drawable;
    };
}
#endif
