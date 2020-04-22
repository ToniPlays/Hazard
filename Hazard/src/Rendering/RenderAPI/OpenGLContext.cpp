#pragma once
#include <hzrpch.h>
#include "OpenGLContext.h"
#include <glad/glad.h>


namespace Hazard {
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(windowHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			HZR_CORE_FATAL("Glad load proc for OpenGL failed");
			exit(0);
		}
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		HZR_CORE_INFO("OpenGL version: {0}.{1}", major, minor);
	}
	void OpenGLContext::ClearColor(Color color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(windowHandle);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void OpenGLContext::SetViewport(int x, int y, int w, int h)
	{
		glViewport(x, y, w, h);
	}
}