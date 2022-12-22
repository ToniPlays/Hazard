#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_PLATFORM_MACOS

#include "Backend/Core/Window.h"
#include "Backend/Input.h"
#include <GLFW/glfw3.h>

namespace HazardRenderer {

    class MacOSWindow : public Window
    {
        friend class Window;
        friend class Input;
    public:
        MacOSWindow(HazardRendererCreateInfo* createInfo);
        virtual ~MacOSWindow();

        virtual void Show() const override;
        virtual void Hide() const override;
        virtual void Close() override;

        virtual void BeginFrame() override;
        virtual void Present() override;

        void SetWindowTitle(const std::string& title) override;
        void SetFullscreen(bool fullscreen) override;

        void SetMaximized(bool maximized) override
        {
            m_WindowData.Maximized = maximized;
            maximized ? glfwMaximizeWindow(m_Window) : glfwRestoreWindow(m_Window);
        }

        void SetMinimized(bool minimized) override
        {
            m_WindowData.Minimized = minimized;
            glfwIconifyWindow(m_Window);
        }

        void SetVSync(bool enabled) override
        {
            m_WindowData.VSync = enabled;
            glfwSwapInterval(enabled);
        };

        void SetPosition(glm::vec2 position) override;
        void SetWindowIcon(uint32_t count, const char** images) override;

        void* GetNativeWindow() const override { return m_Window; }

        unsigned int GetWidth() const override { return m_WindowData.Width; }
        unsigned int GetHeight() const override { return m_WindowData.Height; }

        bool IsVSync() const override { return m_WindowData.VSync; }
        bool IsFocused() const override { return m_WindowData.focus; }
        bool IsMinimized() const override { return m_WindowData.Minimized; }
        bool IsFullscreen() const override { return m_WindowData.Fullscreen; }
        bool IsMaximized() const override { return m_WindowData.Maximized; }
        glm::vec2 GetPosition() override;

        WindowProps& GetWindowInfo() override { return m_WindowData; }
        GraphicsContext* GetContext() const override { return m_Context; };
        Ref<Swapchain> GetSwapchain() override { return m_Context->GetSwapchain(); }
        virtual void SetDebugCallback(RendererMessageCallback callback)
        {
            s_DebugCallback = callback;
            for (auto& m : s_QueueMessages)
                s_DebugCallback(m);

            s_QueueMessages.clear();
        }

    private:
        WindowProps m_WindowData;
        GraphicsContext* m_Context;
        GLFWwindow* m_Window = nullptr;
        static inline RendererMessageCallback s_DebugCallback = nullptr;
        static inline std::vector<RenderMessage> s_QueueMessages;
        void SetCallbacks();
        inline static Window* s_CurrentWindow = nullptr;
    };
}
#endif
