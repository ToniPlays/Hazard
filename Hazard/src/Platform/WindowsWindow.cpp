#pragma once

#include <hzrpch.h>
#include "WindowsWindow.h"
#include "Core/EventSystem/ApplicationEvent.h"
#include "Core/EventSystem/KeyEvent.h"
#include "Core/EventSystem/MouseEvent.h"
#include "glad/glad.h"
#include "Core/Application.h"


#include "imgui.h"
#include "Platform/OpenGl/imgui_impl_glfw.h"
#include "Platform/OpenGl/imgui_impl_opengl3.h"

namespace Hazard {

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		data.Title = props.Title + " | Hazard";
		data.Width = props.Width;
		data.Height = props.Height;

		if(!glfwInit()) {
			HZR_CORE_FATAL("Unable to init GLFW");
			return;
		}
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		
		window = glfwCreateWindow(data.Width, data.Height, data.Title.c_str(), NULL, NULL);

		if (!window) {
			HZR_CORE_FATAL("Unable to create window");
			return;
		}
		
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &data);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		SetVSync(false);

		
		//TODO DEFINE CLEARCOLOR
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glViewport(0, 0, data.Width, data.Height);

		//SET GLFW CALLBACKS

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h) {

			WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
			data.Height = h;
			data.Width = w;
			glViewport(0, 0, data.Width, data.Height);
			WindowResizeEvent event(w, h);
			data.EventCallback(event);
		});
		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
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

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
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
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);

		});
		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}
	void WindowsWindow::OnUpdate()
	{
		
		UpdateType type = Application::Get().getUpdateType();

		type == UpdateType::PollEvents ? glfwPollEvents() : glfwWaitEvents();


		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
		data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return data.VSync;
	}

	void WindowsWindow::SetClearColor(Color color) const
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	Color WindowsWindow::GetClearColor() const
	{
		GLfloat bkColor[4];
		glGetFloatv(GL_COLOR_CLEAR_VALUE, bkColor);

		return Color(bkColor[0], bkColor[1], bkColor[2], bkColor[3]);
	}

	void WindowsWindow::Shutdown()
	{
		glfwTerminate();
		glfwDestroyWindow(window);
	}
}
