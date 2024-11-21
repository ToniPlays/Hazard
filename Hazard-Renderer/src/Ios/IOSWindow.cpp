
#include "IOSWindow.h"
#ifdef HZR_PLATFORM_IOS

#include "Core/Events/Events.h"
#include "vendor/stb_image.h"
#include "Filesystem/File.h"
#include "IOSUtils.h"

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
        m_WindowData.FramebufferScale = { 2.0f, 2.0f };
        
        if (!m_WindowData.EventCallback)
            m_WindowData.EventCallback = [](Event& e) {};
        
        if (info->Renderer == RenderAPI::Auto)
            info->Renderer = RenderAPI::Metal;
        
        SendDebugMessage({ Severity::Info, "Selected API: " + RenderAPIToString(info->Renderer) });
        
        if (info->WindowCount == 1)
        {
            Extent size = IOSUtils::GetWindowSize(0);
            
            if(IOSUtils::IsOrientedAs(0, Orientation::AnyLandscape))
            {
                uint32_t height = size.Height;
                size.Height = size.Width;
                size.Width = height;
            }
            
            HazardWindowCreateInfo windowInfo = info->pWindows[0];
            m_WindowData.Title = windowInfo.Title;
            m_WindowData.Platform = "iOS";
            m_WindowData.SelectedAPI = info->Renderer;
            m_WindowData.HasTitleBar = windowInfo.HasTitlebar;
            m_WindowData.Width = size.Width;
            m_WindowData.Height = size.Height;
            m_WindowData.VSync = info->VSync;
            m_WindowData.ImagesInFlight = info->ImagesInFlight;
            m_WindowData.Window = this;
            
            m_Context = GraphicsContext::Create(&m_WindowData);
            
            m_Context->Init(this, info);
            m_Context->SetClearColor(windowInfo.Color);
            
            m_WindowData.Width = m_Context->GetSwapchain()->GetHeight();
            m_WindowData.Height = m_Context->GetSwapchain()->GetWidth();
            
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

    SafeArea IOSWindow::GetSafeArea() const
    {
        return IOSUtils::GetSafeArea(0);
    }

    void IOSWindow::SetCallbacks()
    {
        IOSUtils::RegisterRotationChange([this](uint8_t orientation) {
            if(orientation == 0) return;
            
            m_WindowData.Orientation = orientation;
            
            if((uint8_t)orientation & ((uint8_t)Orientation::AnyPortrait))
            {
                Extent ext = IOSUtils::GetWindowSize(0);
                m_WindowData.Width = ext.Width;
                m_WindowData.Height = ext.Height;
                
                WindowResizeEvent e(ext.Width, ext.Height, orientation);
                m_WindowData.EventCallback(e);
            }
            else
            {
                Extent ext = IOSUtils::GetWindowSize(0);
                m_WindowData.Width = ext.Height;
                m_WindowData.Height = ext.Width;
                WindowResizeEvent e(ext.Height, ext.Width, orientation);
                m_WindowData.EventCallback(e);
            }
        });
    }
    IOSWindow::~IOSWindow()
    {
        
    }
    void IOSWindow::Close()
    {
        hdelete m_Context;
    }
    std::vector<Resolution> IOSWindow::GetAvailableResolutions() const {
        return { { 1920, 1080 } };
    }

    void IOSWindow::AddDebugCallback(const RendererMessageCallback& callback) {
        
    }
}
#endif

