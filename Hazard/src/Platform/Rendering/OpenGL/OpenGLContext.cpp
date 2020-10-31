#pragma once

#include <hzrpch.h>
#include "OpenGLContext.h"
#include "Hazard/Modules/Renderer/RenderEngine.h"

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
			HZR_ASSERT(false, "Unable to init GLFW");
			return;
		};

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glDebugMessageCallback(RenderEngine::CheckError, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	void OpenGLContext::ClearFrame(Color clearColor) const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	}

	void OpenGLContext::SetViewport(int x, int y, int w, int h) const
	{
		glViewport(x, y, w, h);
	}

	void OpenGLContext::Draw(VertexArray* vertexArray, uint32_t indices, RenderType type)
	{
		vertexArray->Bind();
		switch (type)
		{
		case RenderType::Default:
			glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);
			break;
		case RenderType::Points:
			glPointSize(5.0);
			glDrawArrays(GL_POINTS, 0, indices);
			break;
		case RenderType::Wireframe: 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}
	}

	void OpenGLContext::DrawIndexed(VertexArray* vertexArray, uint32_t indices)
	{
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);
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