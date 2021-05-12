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

	Window* Window::Create(WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(WindowProps& props) {

		if (!glfwInit()) return;

		m_WindowData.Title = props.Title;
		m_WindowData.Platform = "Windows";

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		glfwWindowHint(GLFW_RESIZABLE, true);
		glfwWindowHint(GLFW_MAXIMIZED, props.maximized);

		m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title, 0, 0);

		if (!m_Window) {
			HZR_ASSERT(false, "Window was not initialized");
			return;
		}

		glfwSetWindowUserPointer(m_Window, &m_WindowData);

		switch (Application::GetModule<RenderContext>().GetCurrentAPI()) {

		case RenderAPI::OpenGL:
			m_Context = new OpenGL::OpenGLContext(this, &m_WindowData); 
			break;
		case RenderAPI::Vulkan:
			m_Context = new Vulkan::VKContext(this, &m_WindowData);
			break;
		default:
			HZR_CORE_INFO("RenderContext not supported ({0})", RenderContextCommand::GetContext().GetCurrentAPI());
			break;
		}

		m_WindowData.Renderer = m_Context->GetVersion();

		SetCallbacks();
		glfwShowWindow(m_Window);
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
	void WindowsWindow::SetWindowIcon(const char* smallIcon, const char* bigIcon)
	{
		GLFWimage images[2];
		int sx, sy, bx, by, sChannels, bChannels;
		images[0].pixels = stbi_load(smallIcon, &sx, &sy, &sChannels, 0);
		images[0].width = sx;
		images[0].height = sy;
		images[1].pixels = stbi_load(bigIcon, &bx, &by, &bChannels, 0);
		images[1].width = bx;
		images[1].height = by;

		glfwSetWindowIcon(m_Window, 2, images);
	}
	void WindowsWindow::SetCallbacks()
	{
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int w, int h) {

			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			data.Height = h;
			data.Width = w;
			glViewport(0, 0, w, h);
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