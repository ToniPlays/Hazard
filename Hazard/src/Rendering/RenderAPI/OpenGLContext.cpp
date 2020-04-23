#pragma once
#include <hzrpch.h>
#include "OpenGLContext.h"


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
	std::string OpenGLContext::Get(int value) const
	{
		std::stringstream ss;
		switch (value)
		{
			case SYSTEM_GPU:
				ss << glGetString(GL_RENDERER);
				break;
			case SYSTEM_RENDERER:
				ss << "OpenGL " << GLVersion.major << "." << GLVersion.minor;
				break;
			default:
				ss << "Unknown";
				break;
		}
		return ss.str();
	}
	void OpenGLContext::SetViewport(int x, int y, int w, int h)
	{
		glViewport(x, y, w, h);
	}
}