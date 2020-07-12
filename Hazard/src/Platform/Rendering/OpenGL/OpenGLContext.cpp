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

	void OpenGLContext::ClearFrame(bool useClearColor) const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if(useClearColor)
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		else glClearColor(0, 0, 0, 0);
	}

	void OpenGLContext::SetClearColor(Color color)
	{
		clearColor = color;
	}

	void OpenGLContext::SetViewport(int x, int y, int w, int h) const
	{
		glViewport(x, y, w, h);
	}


	void OpenGLContext::Draw(RenderType type, VertexArray* vertexArray)
	{
		switch (type)
		{
		case RenderType::Default:
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			break;
		case RenderType::Points:
			glPointSize(5.0);
			glDrawArrays(GL_POINTS, 0, vertexArray->GetIndexBuffer()->GetCount());
			break;
		case RenderType::Wireframe: 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}
		if(glGetError() != 0)
			std::cout << "[OpenGL error] " << glGetError() << std::endl;
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