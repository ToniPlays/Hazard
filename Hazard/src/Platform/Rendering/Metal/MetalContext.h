#pragma once

#include "Platform/Rendering/GraphicsContext.h"
#include <Metal/Metal.hpp>

#include <GLFW/glfw3.h>

namespace Hazard::Rendering::Metal
{
    class MetalLayer;

    class MetalContext : public GraphicsContext {
    public:
        MetalContext(WindowProps* props);
        ~MetalContext();

        void Init(Window* window, ApplicationCreateInfo* appInfo) override;
        void SetClearColor(const glm::vec4& color) override { m_ClearColor = color; }
        void SetViewport(int x, int y, int w, int h) override;
        void SwapBuffers() override;
        void Begin() override;
        void End() override;
        void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) override;
        void EndRenderPass(Ref<RenderCommandBuffer> buffer) override;
        void SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth) override;

        void SetErrorListener(const ErrorCallback& callback) override;
        DeviceSpec GetDeviceSpec() const override;

        void AddResizeCallback(const ResizeCallback& callback) override { m_ResizeCallback.push_back(callback); };
        static void Present(MTL::CommandBuffer* buffer);
        static MTL::Device* GetMetalDevice();
        static MTL::CommandQueue* GetMetalCommandQueue() { return s_Instance->m_CommandQueue; }
        
    private:
        GLFWwindow* m_Window;
        glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        static ErrorCallback s_Callback;
        std::vector<ResizeCallback> m_ResizeCallback;
        
        inline static MetalContext* s_Instance;
        
        MetalLayer* m_MetalLayer;
        MTL::CommandQueue* m_CommandQueue;
    };
}
