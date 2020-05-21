#pragma once

#include <hzrpch.h>
#include "OpenGLContext.h"
#include "glad/glad.h"

namespace Hazard {

	GraphicsContext* GraphicsContext::CreateContext(void* window, void* props) {
		return new OpenGLContext((Window*)window, (WindowProps*)props);
	}

	OpenGLContext::OpenGLContext(Window* window, WindowProps* props)
	{
		this->window = (GLFWwindow*)window->GetNativeWindow();

		Init();
	}
	void OpenGLContext::Init() const
	{
		glfwMakeContextCurrent(window);
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			return;
		}
	}
	void OpenGLContext::ClearFrame() const
	{
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void OpenGLContext::SetClearColor(Color color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		clearColor = color;
	}
	void OpenGLContext::SetViewport(int x, int y, int w, int h) const
	{
		glViewport(x, y, w, h);
	};
	Color OpenGLContext::GetClearColor()
	{
		return clearColor;
		
	}
	std::string OpenGLContext::GetVersion() const
	{
		std::stringstream ss;

		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		ss << "OpenGL ";
		ss << major << "." << minor;
		return ss.str();
	}
}
