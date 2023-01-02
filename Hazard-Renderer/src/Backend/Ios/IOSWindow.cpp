
#include "IOSWindow.h"
#ifdef HZR_PLATFORM_IOS

#include "Backend/Core/Events/Events.h"
#include "vendor/stb_image.h"
#include "File.h"

#include <vector>

namespace HazardRenderer
{
    Window* Window::Create(HazardRendererCreateInfo* info)
    {
        if (!Window::IsRenderAPISupported(info->Renderer))
        {
            HZR_THROW("Unsupported rendering API: " + RenderAPIToString(info->Renderer));
        }
        return hnew IOSWindow(info);
    }

    void Window::SendDebugMessage(const RenderMessage& message)
    {
        if (!IOSWindow::s_DebugCallback)
        {
            IOSWindow::s_QueueMessages.push_back(message);
            return;
        }

        for (auto& m : IOSWindow::s_QueueMessages)
        {
            IOSWindow::s_DebugCallback(m);
        }
        IOSWindow::s_QueueMessages.clear();
        IOSWindow::s_DebugCallback(message);
    }

    IOSWindow::IOSWindow(HazardRendererCreateInfo* info)
    {
        HZR_ASSERT(info->pAppInfo != nullptr, "AppInfo cannot be nullptr");

        s_CurrentWindow = this;

        s_DebugCallback = info->pAppInfo->MessageCallback;
        m_WindowData.EventCallback = info->pAppInfo->EventCallback;

        if (!m_WindowData.EventCallback)
            m_WindowData.EventCallback = [](Event& e) {};

        if (info->Renderer == RenderAPI::Auto)
            info->Renderer = RenderAPI::Metal;

        SendDebugMessage({ Severity::Info, "Selected API: " + RenderAPIToString(info->Renderer) });

        if (info->WindowCount == 1)
        {
            HazardWindowCreateInfo windowInfo = info->pWindows[0];
            m_WindowData.Title = windowInfo.Title;
            m_WindowData.Platform = "MacOS";
            m_WindowData.SelectedAPI = info->Renderer;
            m_WindowData.HasTitleBar = windowInfo.HasTitlebar;
            m_WindowData.Width = windowInfo.Width;
            m_WindowData.Height = windowInfo.Height;
            m_WindowData.VSync = info->VSync;
            m_WindowData.ImagesInFlight = info->ImagesInFlight;
            m_WindowData.Window = this;

            m_Context = GraphicsContext::Create(&m_WindowData);

            if (windowInfo.FullScreen)
            {
                
                if (m_WindowData.Width <= 0 || m_WindowData.Height <= 0)
                {
                    
                }
            }
            HZR_ASSERT(m_WindowData.Width > 0, "Window width cannot be less than 0");
            HZR_ASSERT(m_WindowData.Height > 0, "Window height cannot be less than 0");
                
            //Create window

            HZR_ASSERT(m_Window, "Failed to create window");
            
            //Correct for retina displays
            //float x, y;
            //glfwGetWindowContentScale(m_Window, &x, &y);
            //m_WindowData.Width = (float)windowInfo.Width * x;
            //m_WindowData.Height = (float)windowInfo.Height * y;
            

            if (info->pAppInfo->IconCount > 0)
                SetWindowIcon(info->pAppInfo->IconCount, info->pAppInfo->pIcons);

            m_Context->Init(this, info);
            m_Context->SetClearColor(windowInfo.Color);
            m_WindowData.Width = m_Context->GetSwapchain()->GetWidth();
            m_WindowData.Height = m_Context->GetSwapchain()->GetHeight();

            //Center window
            
            SetCallbacks();
            SetVSync(info->VSync);

            Input::Init(*this);
        }
    }
    void IOSWindow::BeginFrame()
    {
        HZR_PROFILE_FUNCTION();
        m_Context->BeginFrame();
    }
    void IOSWindow::Present()
    {
        HZR_PROFILE_FUNCTION();
        m_Context->Present();

        if (!m_WindowData.Minimized) {}
    }
    void IOSWindow::SetCallbacks()
    {
        
    }
    IOSWindow::~IOSWindow()
    {
        
    }
    void IOSWindow::Close()
    {
        hdelete m_Context;
    }
}
#endif

