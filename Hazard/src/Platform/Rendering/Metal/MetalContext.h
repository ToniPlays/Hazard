#pragma once

#include "Platform/Rendering/GraphicsContext.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace Hazard::Rendering::Metal
{
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
        
        static MTL::Device* GetMetalDevice() { return s_Instance->m_Device; }
        static MTL::CommandQueue* GetMetalCommandQueue() { return s_Instance->m_CommandQueue; }
        
    private:
        GLFWwindow* m_Window;
        glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        static ErrorCallback s_Callback;
        std::vector<ResizeCallback> m_ResizeCallback;
        
        inline static MetalContext* s_Instance;
        
        MTL::Device* m_Device;
        MTL::CommandQueue* m_CommandQueue;
    };
}
