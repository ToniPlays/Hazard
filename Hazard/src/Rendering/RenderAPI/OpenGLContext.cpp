#pragma once
#include <hzrpch.h>
#include "OpenGLContext.h"
#include <glad/glad.h>


namespace Hazard {
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(windowHandle);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
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
}