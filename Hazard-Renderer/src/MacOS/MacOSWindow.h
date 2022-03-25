#pragma once

#include "Core/Core.h"

#ifdef HZR_PLATFORM_MACOS
#pragma error Here is MacOS

#include "Platform/System/Window.h"
#include "GLFW/glfw3.h"
#include "Hazard/Math/Color.h"

namespace Hazard::Rendering
{
    class MacOSWindow : public Window {
    public:
        MacOSWindow(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo);
        virtual ~MacOSWindow();

        void BeginFrame() override;
        void Present() override;
        
        void Close() override {}

        void SetWindowTitle(const char* title) override;
        void SetWindowIcon(uint32_t count, const char** images) override;
        void SetPosition(glm::vec2 position, glm::vec2 dragPoint) override;

        void SetVSync(bool enabled) override {
            m_WindowData.VSync = enabled;
            glfwSwapInterval(enabled);
        };
        void SetMaximized(bool maximized) override {
            m_WindowData.maximized = maximized;
            maximized ? glfwMaximizeWindow(m_Window) : glfwRestoreWindow(m_Window);
        }
        void SetMinimized(bool minimized) override {
            m_WindowData.minimized = minimized;
            glfwIconifyWindow(m_Window);
        }
        void SetFullscreen(bool fullscreen) override;
        void* GetNativeWindow() const override { return m_Window; }
        void SetEventCallback(const EventCallbackFn& callback) override {
            m_WindowData.EventCallback = callback;
        }
        unsigned int GetWidth() const override { return m_WindowData.Width; }
        unsigned int GetHeight() const override { return m_WindowData.Height; }

        bool IsVSync() const override { return m_WindowData.VSync; }
        bool IsFocused() const override { return m_WindowData.focus; }
        bool IsMinimized() const override { return m_WindowData.minimized; }
        bool IsFullscreen() const override { return m_WindowData.fullscreen; }
        bool IsMaximized() const override { return m_WindowData.maximized; }
        WindowProps GetWindowInfo() override { return m_WindowData; }

        GraphicsContext* GetContext() const override { return m_Context; };

    private:
        WindowProps m_WindowData;
        GraphicsContext* m_Context;
        GLFWwindow* m_Window = nullptr;
        void SetCallbacks();
    };
}
#endif
