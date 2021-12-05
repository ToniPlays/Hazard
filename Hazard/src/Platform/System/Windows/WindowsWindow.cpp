#pragma once

#include <hzrpch.h>
#include "WindowWindow.h"
#include "Hazard/Events/Input.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "Platform/Rendering/OpenGL/OpenGLContext.h"
#include "Platform/Rendering/Vulkan/VulkanContext.h"

#include <stb_image.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Hazard::Rendering {

	Window* Window::Create(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo) 
	{
		return new WindowsWindow(info, appInfo);
	}

	WindowsWindow::WindowsWindow(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo) 
	{
		if (!glfwInit()) {
			HZR_THROW("Failed to init GLFW");
		}

		m_WindowData.Title = appInfo->AppName + " " + appInfo->BuildVersion + " " + RenderContext::APIToString(info->Renderer);
		m_WindowData.Platform = "Windows";
		m_WindowData.Width = info->Width;
		m_WindowData.Height = info->Height;
		m_WindowData.VSync = info->VSync;
		m_WindowData.ImagesInFlight = info->ImagesInFlight;

		m_Context = GraphicsContext::Create(info->Renderer, &m_WindowData);

		glfwWindowHint(GLFW_RESIZABLE, info->Resizable);
		glfwWindowHint(GLFW_DECORATED, info->Decorated);


		GLFWmonitor* monitor = NULL;

		if (info->FullScreen) {
			monitor = glfwGetPrimaryMonitor();
			if (m_WindowData.Width <= 0 || m_WindowData.Height <= 0)
			{
				m_WindowData.Width = glfwGetVideoMode(monitor)->width;
				m_WindowData.Height = glfwGetVideoMode(monitor)->height;
			}
		}
		m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title.c_str(), monitor, NULL);
		
		if (!m_Window) {
			HZR_CORE_INFO("Failed to create window");
		}

		if (appInfo->IconCount > 0)
			SetWindowIcon(appInfo->IconCount, appInfo->Icons);

		m_Context->Init(this, appInfo);
		m_Context->SetClearColor(info->Color);

		m_WindowData.deviceSpecs = m_Context->GetDeviceSpec();

		glfwSetWindowUserPointer(m_Window, &m_WindowData);

		if (info->Maximized)
			glfwMaximizeWindow(m_Window);

		glfwShowWindow(m_Window);
		SetCallbacks();
		SetVSync(info->VSync);
	}
	void WindowsWindow::OnBeginFrame()
	{
		m_Context->Begin();
	}
	void WindowsWindow::OnEndFrame()
	{
		m_Context->End();
		if (!m_WindowData.minimized)
			m_Context->SwapBuffers();
		Input::Update();
		glfwPollEvents();
	}
	void WindowsWindow::SetWindowTitle(const char* title)
	{
		m_WindowData.Title = title;
		glfwSetWindowTitle(m_Window, title);
	}
	void WindowsWindow::SetWindowIcon(uint32_t count, const char** images)
	{
		std::vector<GLFWimage> glfwImages(count);

		for (uint32_t i = 0; i < count; i++) {
			GLFWimage img = glfwImages[i];
			const std::string& path = File::GetFileAbsolutePath(images[i]);

			int sx, sy, sChannels, bChannels;
			img.pixels = stbi_load(path.c_str(), &sx, &sy, &sChannels, 3);
			HZR_ASSERT(img.pixels, "Could not load Window Icon {0}", images[i]);
			img.width = sx;
			img.height = sy;
		}
		glfwSetWindowIcon(m_Window, count, glfwImages.data());

		for (auto image : glfwImages) {
			stbi_image_free(image.pixels);
		}
	}
	void WindowsWindow::SetPosition(glm::vec2 position, glm::vec2 dragPoint)
	{
		int x, y;
		glfwGetWindowPos(m_Window, &x, &y);

		glm::vec2 delta = position - dragPoint;
		HZR_CORE_INFO("{0} x {1}", delta.x, delta.y);
		position.x += (float)x;
		position.y += (float)y;

		glfwSetWindowPos(m_Window, x + delta.x, y + delta.y);
	}
	void WindowsWindow::SetFullscreen(bool fullscreen)
	{
		m_WindowData.fullscreen = fullscreen;
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	void WindowsWindow::SetCallbacks()
	{
		HZR_PROFILE_FUNCTION();
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int w, int h) {

			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			data.Height = h;
			data.Width = w;
			Application::GetModule<RenderContext>().GetContext().SetViewport(0, 0, w, h);

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
			Input::UpdateKey(key, action);
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
			data.minimized = minimized;
			Application::GetData().Minimized = minimized == 1;
			});
	}

	WindowsWindow::~WindowsWindow() 
	{
		HZR_PROFILE_FUNCTION();
		delete m_Context;
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}