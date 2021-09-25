#pragma once

#include <hzrpch.h>
#include "WindowWindow.h"
#include "Hazard/Events/Input.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "Platform/Rendering/OpenGL/OpenGLContext.h"
#include "Platform/Rendering/Vulkan/VulkanContext.h"

#include <glad/glad.h>
#include <stb_image.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Hazard::Rendering {

	Window* Window::Create(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo) {
		return new WindowsWindow(info, appInfo);
	}

	WindowsWindow::WindowsWindow(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo) {

		HZR_PROFILE_FUNCTION();

		if (!glfwInit()) {
			HZR_THROW("Failed to init GLFW");
		}

		m_WindowData.Title = appInfo->AppName + " " + appInfo->BuildVersion + " " + RenderContext::APIToString(info->Renderer);
		m_WindowData.Platform = "Windows";
		m_WindowData.Width = info->Width;
		m_WindowData.Height = info->Height;
		m_WindowData.ImagesInFlight = info->ImagesInFlight;

		switch (info->Renderer)
		{
		case RenderAPI::OpenGL:
			m_Context = new OpenGL::OpenGLContext(&m_WindowData);
			break;
			case RenderAPI::Vulkan:
				m_Context = new Vulkan::VulkanContext(&m_WindowData);
				break;
		default:
			HZR_THROW("RenderContext not supported: ", RenderContext::APIToString(api));
			break;
		}

		glfwWindowHint(GLFW_RESIZABLE, info->Resizable);
		glfwWindowHint(GLFW_DECORATED, info->Decorated);

		if (info->FullScreen) {
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			if (m_WindowData.Width <= 0 || m_WindowData.Height <= 0) {
				m_WindowData.Width = glfwGetVideoMode(monitor)->width;
				m_WindowData.Height = glfwGetVideoMode(monitor)->height;

			}
			m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title.c_str(), monitor, NULL);
		}
		else {
			m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title.c_str(), NULL, NULL);
		}

		if (!m_Window) {
			HZR_CORE_INFO("Failed to create window");
		}
		m_Context->SetClearColor(info->Color);
		m_Context->Init(this, appInfo);
		m_WindowData.deviceSpecs = m_Context->GetDeviceSpec();

		glfwSetWindowUserPointer(m_Window, &m_WindowData);

		if(info->Maximized)
			glfwMaximizeWindow(m_Window);

		glfwShowWindow(m_Window);
		SetCallbacks();
		SetVSync(info->VSync);

		HZR_CORE_INFO(m_Context->GetDeviceSpec().name);

	}
	void WindowsWindow::OnUpdate()
	{
		m_Context->SwapBuffers();
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
			GLFWimage img = glfwImages[i];
			int sx, sy, sChannels, bChannels;
			img.pixels = stbi_load(images[i].c_str(), &sx, &sy, &sChannels, 0);
			img.width = sx;
			img.height = sy;
		}
		glfwSetWindowIcon(m_Window, count, glfwImages.data());
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
			});
	}

	WindowsWindow::~WindowsWindow() {

		delete m_Context;
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}