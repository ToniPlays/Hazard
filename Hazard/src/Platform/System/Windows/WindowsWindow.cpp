#pragma once

#include <hzrpch.h>
#include "WindowWindow.h"
#include "Hazard/Events/Input.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

#include "Platform/Rendering/OpenGL/OpenGLContext.h"
#include "Platform/Rendering/Vulkan/VKContext.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Hazard::Rendering {

	Window* Window::Create(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo) {
		return new WindowsWindow(info, appInfo);
	}

	WindowsWindow::WindowsWindow(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo) {

		HZR_PROFILE_FUNCTION();
		if (glfwInit() != GLFW_TRUE) {
			HZR_THROW("Failed to init GLFW");
		}

		m_WindowData.Title = appInfo->appName.c_str();
		m_WindowData.Platform = "Windows";
		m_WindowData.Width = info->width;
		m_WindowData.Height = info->height;

		RenderAPI api = Application::GetModule<RenderContext>()->GetCurrentAPI();
		switch (api) {

		case RenderAPI::OpenGL:
			m_Context = new OpenGL::OpenGLContext(&m_WindowData); 
			break;
		case RenderAPI::Vulkan:
			m_Context = new Vulkan::VKContext(&m_WindowData);
			break;
		default:
			std::stringstream ss;
			ss << "RenderContext not supported: " << RenderContext::APIToString(api);
			HZR_THROW(ss.str());
		}

		glfwWindowHint(GLFW_RESIZABLE, true);
		glfwWindowHint(GLFW_MAXIMIZED, info->fullScreen);

		m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title, 0, 0);

		if (!m_Window) {
			HZR_THROW("Failed to create window");
		}

		m_Context->Init(this, appInfo);

		glfwSetWindowUserPointer(m_Window, &m_WindowData);
		m_WindowData.Renderer = m_Context->GetDeviceSpec().renderer;

		glfwShowWindow(m_Window);
		SetCallbacks();
		SetVSync(info->VSync);
	}
	void WindowsWindow::OnUpdate(Color color) {

		glfwSwapBuffers(m_Window);
		m_Context->ClearFrame(color);
		glfwPollEvents();
	}
	void WindowsWindow::SetWindowTitle(const char* title)
	{
		m_WindowData.Title = title;
		glfwSetWindowTitle(m_Window, title);
	}
	void WindowsWindow::SetWindowIcon(uint32_t count, std::string* images)
	{
		std::vector<GLFWimage> glfwImages(count);

		for (uint32_t i = 0; i < count; i++) {

			int sx, sy, sChannels, bChannels;
			glfwImages.at(i).pixels = stbi_load(images[i].c_str(), &sx, &sy, &sChannels, 0);
			glfwImages.at(i).width = sx;
			glfwImages.at(i).height = sy;
		}
		glfwSetWindowIcon(m_Window, count, glfwImages.data());
	}
	void WindowsWindow::SetCallbacks()
	{
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int w, int h) {

			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			data.Height = h;
			data.Width = w;
			Application::GetModule<RenderContext>()->GetContext().SetViewport(0, 0, w, h);
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
				MouseButtonPressedEvent event(button);
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
		});
	}

	WindowsWindow::~WindowsWindow() {

		delete m_Context;
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}