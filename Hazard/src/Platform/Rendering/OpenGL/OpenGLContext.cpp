#pragma once

#include <hzrpch.h>
#include "OpenGLContext.h"
#include <glad/glad.h>

namespace Hazard {

	ErrorCallback OpenGLContext::callback;

	GraphicsContext* GraphicsContext::Create(void* window, void* props) {
		return new OpenGLContext((Window*)window, (WindowProps*)props);
	}

	OpenGLContext::OpenGLContext(Window* window, WindowProps* props)
	{
		this->window = (GLFWwindow*)window->GetNativeWindow();
		Init();
	}

	OpenGLContext::~OpenGLContext()
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_DEBUG_OUTPUT);
		glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	void OpenGLContext::Init() const
	{
		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			HZR_ASSERT(false, "Unable to init GLFW");
			return;
		};

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		
		glDebugMessageCallback(OpenGLContext::OnError, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	void OpenGLContext::ClearFrame(Color clearColor) const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0,
			GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Clearing");
	}

	void OpenGLContext::SetViewport(int x, int y, int w, int h) const
	{
		glViewport(x, y, w, h);
	}

	void OpenGLContext::DrawArray(VertexArray* array) const
	{
		array->EnableAll();
		glDrawElements(GL_TRIANGLES, array->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLContext::SetErrorListener(const ErrorCallback& callback)
	{
		OpenGLContext::callback = callback;
		HZR_CORE_WARN(GetVersion() + " added debug callback");
	}

	void GLAPIENTRY OpenGLContext::OnError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) 
	{
		HZR_CORE_WARN(message);

		if (callback) {
			ErrorData data(message, type);
			OpenGLContext::callback(data);
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