#pragma once
#include <hzrpch.h>
#include "WindosWindow.h"
#include "glad/glad.h"

namespace Hazard {

	Window* Window::Create(WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(WindowProps& props) {
		Init(props);
	}

	void WindowsWindow::SetCallbacks()
	{
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h) {

			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			data.Height = h;
			data.Width = w;
			glViewport(0, 0, w, h);
			WindowResizeEvent event(w, h);
			data.EventCallback(event);
			});
		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {

			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
			});
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
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

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
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
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);

			});
		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
			});
	}

	void WindowsWindow::Init(WindowProps& props) {

		windowData.Title = props.Title;
		windowData.Width = props.Width;
		windowData.Height = props.Height;
		windowData.Platform = "Windows";

		if (!glfwInit()) {
			return;
		}
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(windowData.Width, windowData.Height, windowData.Title.c_str(), NULL, NULL);


		if (!window) {
			return;
		}

		glfwSetWindowUserPointer(window, &windowData);
		context = GraphicsContext::CreateContext(this, &props);

		windowData.Renderer = context->GetVersion();

		SetCallbacks();
		SetClearColor(Color::FromHex("#1473c5"));
	}
	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		context->ClearFrame();
	}

	void WindowsWindow::SetClearColor(Color color) const
	{
		context->SetClearColor(color);
	}
	Color WindowsWindow::GetClearColor() const
	{
		return context->GetClearColor();
	}
	void WindowsWindow::SetWindowTitle(std::string title)
	{
		windowData.Title = title;
		glfwSetWindowTitle(window, title.c_str());
	}
}