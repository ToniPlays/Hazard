#pragma once

#include "Core/Core.h"
#ifdef HZR_PLATFORM_IOS

#include "Core/Window.h"
#include "Input.h"

namespace HazardRenderer
{
    class IOSWindow : public Window
    {
        friend class Window;
        friend class Input;
    public:
        IOSWindow(HazardRendererCreateInfo* createInfo);
        virtual ~IOSWindow();

        virtual void Show() const override {};
        virtual void Hide() const override {};
        virtual void Close() override;

        virtual void BeginFrame() override;
        virtual void Present() override;

        void SetWindowTitle(const std::string& title) override {};
        void SetFullscreen(bool fullscreen) override {};

        void SetMaximized(bool maximized) override {}

        void SetMinimized(bool minimized) override {}

        void SetVSync(bool enabled) override {};

        void SetPosition(glm::vec2 position) override {};
        void SetWindowIcon(uint32_t count, const char** images) override {};

        void* GetNativeWindow() const override { return m_Window; }

        unsigned int GetWidth() const override { return m_WindowData.Width; }
        unsigned int GetHeight() const override { return m_WindowData.Height; }

        bool IsVSync() const override { return m_WindowData.VSync; }
        bool IsFocused() const override { return m_WindowData.focus; }
        bool IsMinimized() const override { return m_WindowData.Minimized; }
        bool IsFullscreen() const override { return m_WindowData.Fullscreen; }
        bool IsMaximized() const override { return m_WindowData.Maximized; }
        glm::vec2 GetPosition() override { return { 0, 0 }; };
        
        std::vector<Resolution> GetAvailableResolutions() const override;
        
        void SetResolution(const Resolution& resolution) override {};

        WindowProps& GetWindowInfo() override { return m_WindowData; }
        GraphicsContext* GetContext() const override { return m_Context; };
        Ref<Swapchain> GetSwapchain() override { return m_Context->GetSwapchain(); }
        void SetDebugCallback(const RendererMessageCallback& callback) override
        {
            s_DebugCallback = callback;
            
            for (auto& m : s_QueueMessages)
                s_DebugCallback(m);
            
            s_QueueMessages.clear();
        }
    private:
        void SetCallbacks();
        
    private:
        WindowProps m_WindowData;
        GraphicsContext* m_Context;
        void* m_Window = nullptr;
        static inline RendererMessageCallback s_DebugCallback = nullptr;
        static inline std::vector<RenderMessage> s_QueueMessages;
        
        inline static Window* s_CurrentWindow = nullptr;
    };
}
#endif
