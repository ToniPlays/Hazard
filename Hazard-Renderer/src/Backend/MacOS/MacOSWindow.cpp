#include "MacOSWindow.h"

#ifdef HZR_PLATFORM_MACOS

#include "File.h"
#include "Backend/Core/Events.h"
#include "Renderer/RenderCommand.h"
#include "Backend/Core/Renderer.h"

#include "vendor/stb_image.h"
#include <vector>

namespace HazardRenderer
{
    Window* Window::Create(HazardRendererCreateInfo* info)
    {
        if (!Window::IsRenderAPISupported(info->Renderer)) {
            HZR_THROW("Unsupported rendering API: " + RenderAPIToString(info->Renderer));
        }
        return hnew MacOSWindow(info);
    }

    void Window::SendDebugMessage(const RenderMessage& message)
    {
        if (MacOSWindow::s_DebugCallback == nullptr) return;

        RenderMessage msg;
        msg.Severity = message.Severity;
        msg.Message = "[Hazard-Renderer]: " + message.Message;

        MacOSWindow::s_DebugCallback(msg);
    }

    MacOSWindow::MacOSWindow(HazardRendererCreateInfo* info)
    {
        HZR_ASSERT(info->pAppInfo != nullptr, "AppInfo cannot be nullptr");
        HZR_ASSERT(glfwInit(), "Failed to initialize GLFW");
        s_DebugCallback = info->pAppInfo->MessageCallback;
        m_WindowData.EventCallback = info->pAppInfo->EventCallback;

        if (!m_WindowData.EventCallback) {
            m_WindowData.EventCallback = [](Event& e) {};
        }

        if (info->Renderer == RenderAPI::Auto)
        {
            info->Renderer = RenderAPI::Metal;
        }

        SendDebugMessage({ Severity::Info, "Selected API: " + RenderAPIToString(info->Renderer) });

        if(info->WindowCount == 1)
        {
            HazardWindowCreateInfo windowInfo = info->pWindows[0];
            m_WindowData.Title = windowInfo.Title;
            m_WindowData.Platform = "Windows";
            m_WindowData.SelectedAPI = info->Renderer;
            m_WindowData.Width = windowInfo.Width;
            m_WindowData.Height = windowInfo.Height;
            m_WindowData.VSync = info->VSync;
            m_WindowData.ImagesInFlight = info->ImagesInFlight;
            m_WindowData.Window = this;

            m_Context = GraphicsContext::Create(&m_WindowData);

            glfwWindowHint(GLFW_RESIZABLE, windowInfo.Resizable);
            glfwWindowHint(GLFW_DECORATED, windowInfo.Decorated);
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

            GLFWmonitor* monitor = NULL;

            if (windowInfo.FullScreen)
            {
                monitor = glfwGetPrimaryMonitor();
                if (m_WindowData.Width <= 0 || m_WindowData.Height <= 0)
                {
                    m_WindowData.Width = glfwGetVideoMode(monitor)->width;
                    m_WindowData.Height = glfwGetVideoMode(monitor)->height;
                }
            }
            HZR_ASSERT(m_WindowData.Width > 0, "Window width cannot be less than 0");
            HZR_ASSERT(m_WindowData.Height > 0, "Window height cannot be less than 0");

            //Create window
            m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title.c_str(), monitor, NULL);

            HZR_ASSERT(m_Window, "Failed to create window");

            //Center window
            monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowPos(m_Window, mode->width / 2 - m_WindowData.Width / 2, mode->height / 2 - m_WindowData.Height / 2);


            if (info->pAppInfo->IconCount > 0)
                SetWindowIcon(info->pAppInfo->IconCount, info->pAppInfo->pIcons);

            m_Context->Init(this, info);
            m_Context->SetClearColor(windowInfo.Color);

            glfwSetWindowUserPointer(m_Window, &m_WindowData);

            if (windowInfo.Maximized)
                glfwMaximizeWindow(m_Window);

            SetCallbacks();
            SetVSync(info->VSync);

            RenderCommand::Init(this);
            Renderer::Init();
        }
    }
    void MacOSWindow::BeginFrame()
    {
        m_Context->BeginFrame();
    }
    void MacOSWindow::Present()
    {
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

        for (uint32_t i = 0; i < count; i++) {
            GLFWimage img = glfwImages[i];
            const std::string& path = File::GetFileAbsolutePath(std::string(images[i]));

            int sx, sy, sChannels;
            img.pixels = stbi_load(path.c_str(), &sx, &sy, &sChannels, 3);
            HZR_ASSERT(img.pixels != 0, "Could not load Window Icon");
            img.width = sx;
            img.height = sy;
        }
        glfwSetWindowIcon(m_Window, count, glfwImages.data());

        for (auto image : glfwImages) {
            stbi_image_free(image.pixels);
        }
    }

    glm::vec2 MacOSWindow::GetPosition()
    {
        int x, y;
        glfwGetWindowPos(m_Window, &x, &y);
        return { x, y };
    }

    void MacOSWindow::SetPosition(glm::vec2 position)
    {
        glfwSetWindowPos(m_Window, position.x, position.y);
    }

    void MacOSWindow::SetFullscreen(bool fullscreen)
    {
        m_WindowData.Fullscreen = fullscreen;
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    void MacOSWindow::SetCallbacks()
    {
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int w, int h) {

            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            float x, y;
            glfwGetWindowContentScale((GLFWwindow*)data.Window->GetNativeWindow(), &x, &y);
            data.Width = (float)w * x;
            data.Height = (float)h * y;
            data.Window->GetContext()->SetViewport(0, 0, data.Width, data.Height);

            WindowResizeEvent event(data.Width, data.Height);
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
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(key, 1);
                data.EventCallback(event);
                break;
            }
            }
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                data.EventCallback(event);
                break;
            }
            }
            });
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);

            });
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
            });
        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focus) {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            data.focus = focus;
            WindowFocusEvent event(focus);
            data.EventCallback(event);
            });
        glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int minimized) {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            data.Minimized = minimized;
            });
    }
    MacOSWindow::~MacOSWindow()
    {
        Close();
        hdelete m_Context;
        glfwTerminate();
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
        glfwDestroyWindow(m_Window);
}

}
#endif
