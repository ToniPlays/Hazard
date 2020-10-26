#pragma once

#include <hzrpch.h>
#include "WindowWindow.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace Hazard {

	Window* Window::Create(WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(WindowProps& props) {

		if (!glfwInit()) {
			return;
		}

		windowData.Title = props.Title;
		windowData.Platform = "Windows";


		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_MAXIMIZED, props.maximized ? GLFW_TRUE : GLFW_FALSE);

		window = glfwCreateWindow(windowData.Width, windowData.Height, windowData.Title.c_str(), 0, 0);

		if (!window) {
			HZR_ASSERT(false, "Window was not initialized");
			return;
		}

		glfwSetWindowUserPointer(window, &windowData);
		context = GraphicsContext::Create(this, &windowData);
		windowData.Renderer = context->GetVersion();

		std::string hex = "#222222FF";
		SetClearColor(Color::FromHex(hex));
		SetCallbacks();
		SetVSync(true);


		glfwShowWindow(window);
		glfwGetWindowSize(window, &windowData.Width, &windowData.Height);
	}
	void WindowsWindow::OnUpdate() {

		glfwSwapBuffers(window);
		context->ClearFrame(true);
		IsFocused() ? glfwPollEvents() : glfwWaitEventsTimeout(1.0f / 24.0f);
	}
	void WindowsWindow::SetWindowTitle(std::string title)
	{
		windowData.Title = title;
		glfwSetWindowTitle(window, title.c_str());
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

		glfwSetWindowIcon(window, 2, images);
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
		glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focus) {
			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			data.focus = focus;
			WindowFocusEvent event(focus);
			data.EventCallback(event);
		});
	}
	void WindowsWindow::SetClearColor(Color color) const
	{
		context->SetClearColor(color);
	}

	WindowsWindow::~WindowsWindow() {

		glfwDestroyWindow(window);
		glfwTerminate();
	}
}