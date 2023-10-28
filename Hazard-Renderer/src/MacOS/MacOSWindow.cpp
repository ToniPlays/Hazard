
#include "MacOSWindow.h"
#ifdef HZR_PLATFORM_MACOS

#include "Core/Events/Events.h"
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
        return hnew MacOSWindow(info);
    }

    void Window::SendDebugMessage(const RenderMessage& message)
    {
        if (!MacOSWindow::s_DebugCallback)
        {
            MacOSWindow::s_QueueMessages.push_back(message);
            return;
        }

        for (auto& m : MacOSWindow::s_QueueMessages)
        {
            MacOSWindow::s_DebugCallback(m);
        }
        MacOSWindow::s_QueueMessages.clear();
        MacOSWindow::s_DebugCallback(message);
    }

    MacOSWindow::MacOSWindow(HazardRendererCreateInfo* info)
    {
        HZR_ASSERT(info->pAppInfo != nullptr, "AppInfo cannot be nullptr");
        HZR_ASSERT(glfwInit(), "Failed to initialize GLFW");

        s_CurrentWindow = this;

        s_DebugCallback = info->pAppInfo->MessageCallback;
        m_WindowData.EventCallback = info->pAppInfo->EventCallback;

        if (!m_WindowData.EventCallback)
        {
            m_WindowData.EventCallback = [](Event& e) {};
        }

        if (info->Renderer == RenderAPI::Auto)
            info->Renderer = RenderAPI::Metal;

        SendDebugMessage({ Severity::Info, "Selected API: " + RenderAPIToString(info->Renderer) });

        if (info->WindowCount == 1)
        {
            HazardWindowCreateInfo windowInfo = info->pWindows[0];
            m_WindowData.Title = windowInfo.Title;
            m_WindowData.Platform = "Windows";
            m_WindowData.SelectedAPI = info->Renderer;
            m_WindowData.HasTitleBar = windowInfo.HasTitlebar;
            m_WindowData.Width = windowInfo.Extent.Width;
            m_WindowData.Height = windowInfo.Extent.Height;
            m_WindowData.VSync = info->VSync;
            m_WindowData.ImagesInFlight = info->ImagesInFlight;
            m_WindowData.Window = this;

            glfwWindowHint(GLFW_DECORATED, windowInfo.HasTitlebar);
            //glfwWindowHint(GLFW_TITLEBAR, windowInfo.HasTitlebar);
            glfwWindowHint(GLFW_RESIZABLE, windowInfo.Resizable);
            glfwWindowHint(GLFW_MAXIMIZED, windowInfo.Maximized);
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            GLFWmonitor* monitor = NULL;

            if (windowInfo.FullScreen)
            {
                if (m_WindowData.Width <= 0 || m_WindowData.Height <= 0)
                {
                    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                    m_WindowData.Width = mode->width;
                    m_WindowData.Height = mode->height;
                    m_WindowData.RefreshRate = mode->refreshRate;
                }
            }
            HZR_ASSERT(m_WindowData.Width > 0, "Window width cannot be less than 0");
            HZR_ASSERT(m_WindowData.Height > 0, "Window height cannot be less than 0");

            //Create window
            m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title.c_str(), monitor, NULL);

            HZR_ASSERT(m_Window, "Failed to create window");

            if (info->pAppInfo->IconCount > 0)
                SetWindowIcon(info->pAppInfo->IconCount, info->pAppInfo->pIcons);

            if (monitor == nullptr)
                monitor = glfwGetPrimaryMonitor();

            glfwGetWindowContentScale(m_Window, &m_WindowData.FramebufferScale.x, &m_WindowData.FramebufferScale.y);

            m_Context = GraphicsContext::Create(&m_WindowData);
            m_Context->Init(this, info);
            m_Context->SetClearColor(windowInfo.Color);
            
            m_WindowData.Width = m_Context->GetSwapchain()->GetWidth() * m_WindowData.FramebufferScale.x;
            m_WindowData.Height = m_Context->GetSwapchain()->GetHeight() * m_WindowData.FramebufferScale.y;
            m_WindowData.RefreshRate = glfwGetVideoMode(monitor)->refreshRate;

            //Center window
            monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowPos(m_Window, mode->width / 2.0f - m_WindowData.Width / 2.0f, mode->height / 2.0f - m_WindowData.Height / 2.0f);

            glfwSetWindowUserPointer(m_Window, &m_WindowData);
            for (uint32_t i = 0; i < GLFW_JOYSTICK_LAST; i++)
                glfwSetJoystickUserPointer(i, &m_WindowData);

            m_WindowData.Width = m_Context->GetSwapchain()->GetWidth();
            m_WindowData.Height = m_Context->GetSwapchain()->GetHeight();
            
            SetCallbacks();
            SetVSync(info->VSync);

            Input::Init(*this);
        }
    }
    void MacOSWindow::BeginFrame()
    {
        HZR_PROFILE_FUNCTION();
        m_Context->BeginFrame();
    }
    void MacOSWindow::Present()
    {
        HZR_PROFILE_FUNCTION();
        m_Context->Present();

        if (!m_WindowData.Minimized) {}
        glfwPollEvents();
    }

    void MacOSWindow::SetWindowTitle(const std::string& title)
    {
        m_WindowData.Title = title;
        glfwSetWindowTitle(m_Window, title.c_str());
    }

    void MacOSWindow::SetWindowIcon(uint32_t count, const char** images)
    {
        std::vector<GLFWimage> glfwImages(count);

        for (uint32_t i = 0; i < count; i++)
        {
            GLFWimage img = glfwImages[i];
            const std::filesystem::path& path = File::GetFileAbsolutePath(std::string(images[i]));

            int sx, sy, sChannels;
            img.pixels = stbi_load(path.string().c_str(), &sx, &sy, &sChannels, 3);
            HZR_ASSERT(img.pixels != 0, "Could not load Window Icon");
            img.width = sx;
            img.height = sy;
        }
        glfwSetWindowIcon(m_Window, count, glfwImages.data());

        for (auto& image : glfwImages)
            stbi_image_free(image.pixels);
    }

    glm::vec2 MacOSWindow::GetPosition()
    {
        int x, y;
        glfwGetWindowPos(m_Window, &x, &y);
        return { x, y };
    }

    std::vector<Resolution> MacOSWindow::GetAvailableResolutions() const
    {
        int count = 0;
        const GLFWvidmode* modePtr = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

        std::vector<Resolution> result;

        for (uint32_t i = 0; i < count; i++)
        {
            const GLFWvidmode mode = modePtr[i];
            Resolution resolution = {};
            resolution.Width = mode.width;
            resolution.Height = mode.height;
            resolution.RefreshRate = mode.refreshRate;
            result.push_back(resolution);
        }
        return result;
    }

    void MacOSWindow::SetResolution(const Resolution& resolution)
    {
        int x, y;
        glfwGetWindowPos(m_Window, &x, &y);
        glfwSetWindowMonitor(m_Window, NULL, x, y, resolution.Width, resolution.Height, resolution.RefreshRate);
    }

    void MacOSWindow::SetPosition(glm::vec2 position)
    {
        glfwSetWindowPos(m_Window, position.x, position.y);
    }

    void MacOSWindow::SetFullscreen(bool fullscreen)
    {
        m_WindowData.Fullscreen = fullscreen;
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        glfwSetWindowMonitor(m_Window, monitor, 0, 0, m_WindowData.Width, m_WindowData.Height, m_WindowData.RefreshRate);
    }
    void MacOSWindow::SetCallbacks()
    {
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int w, int h) {

            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            data.Width = w * data.FramebufferScale.x;
            data.Height = h * data.FramebufferScale.y;
            
            data.Window->GetContext()->GetSwapchain()->Resize(data.Width, data.Height);

            WindowResizeEvent event(w, h);
            data.EventCallback(event);

        });
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {

            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);

        });
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent e(key, 0);
                    Input::OnEvent(e);
                    data.EventCallback(e);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent e(key);
                    Input::OnEvent(e);
                    data.EventCallback(e);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent e(key, 1);
                    Input::OnEvent(e);
                    data.EventCallback(e);
                    break;
                }
            }
            Input::UpdateKey(key, action);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent e(button);
                    Input::OnEvent(e);
                    data.EventCallback(e);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent e(button);
                    Input::OnEvent(e);
                    data.EventCallback(e);
                    break;
                }
            }
        });
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent e((float)xOffset, (float)yOffset);
            Input::OnEvent(e);
            data.EventCallback(e);
        });
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            MouseMovedEvent e((float)xPos, (float)yPos);
            Input::OnEvent(e);
            data.EventCallback(e);
        });
        glfwSetJoystickCallback([](int device, int status) {
            if (status == GLFW_CONNECTED)
            {
                WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer((GLFWwindow*)s_CurrentWindow->GetNativeWindow());
                GamepadConnectedEvent e(device);
                data.EventCallback(e);

                Input::ConnectGamepad(device);
            }
            else if (status == GLFW_DISCONNECTED)
            {
                WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer((GLFWwindow*)s_CurrentWindow->GetNativeWindow());
                GamepadDisconnectedEvent event(device);
                data.EventCallback(event);
                Input::DisconnectGamepad(device);
            }
        });
        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focus) {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            data.focus = focus;

            WindowFocusEvent e(focus);
            data.EventCallback(e);
        });
        glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int minimized) {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            data.Minimized = minimized;
        });
        /*
        glfwSetTitlebarHitTestCallback(m_Window, [](GLFWwindow* window, int x, int y, int* hit) {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            WindowTitleBarHitTestEvent e(x, y, *hit);
            data.EventCallback(e);
            });
            */
    }
    MacOSWindow::~MacOSWindow()
    {

    }
    void MacOSWindow::Show() const
    {
        glfwShowWindow(m_Window);
    }
    void MacOSWindow::Hide() const
    {
        glfwHideWindow(m_Window);
    }
    void MacOSWindow::Close()
    {
        hdelete m_Context;
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
}
#endif