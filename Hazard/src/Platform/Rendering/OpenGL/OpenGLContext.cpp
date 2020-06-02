#pragma once

#include <hzrpch.h>
#include "OpenGLContext.h"

namespace Hazard {
	GraphicsContext* GraphicsContext::Create(void* window, void* props) {
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
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			HZR_ASSERT(false, "Unable to init GLFW")
			return;
		};

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);

		HazardLoop::GetAppInfo().SetValue("Renderer", GetVersion());
	}

	void OpenGLContext::ClearFrame() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLContext::SetClearColor(Color color)
	{
		clearColor = color;
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLContext::SetViewport(int x, int y, int w, int h) const
	{
		glViewport(x, y, w, h);
	}


	void OpenGLContext::Draw(RenderType type, Mesh* mesh)
	{
		switch (type)
		{
		case Hazard::RenderType::Default:
			glDrawElements(GL_TRIANGLES, mesh->GetIndicesLength(), GL_UNSIGNED_INT, nullptr);
			break;
		case Hazard::RenderType::Points:
			glPointSize(5.0);
			glDrawArrays(GL_POINTS, 0, mesh->GetVerticesLength());
			break;
		}
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