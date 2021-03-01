#pragma once

#include <hzrpch.h>
#include "OpenGLContext.h"
#include <glad/glad.h>

namespace Hazard::Rendering {

	ErrorCallback OpenGLContext::callback;


	/*void APIENTRY OpenGLContext::OnError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		std::stringstream ss;

		ss << "[OpenGL]: " << message;
		ErrorCallback callback = ((OpenGLContext*)userParam)->callback;

		if (callback) {
			ErrorData data(ss.str(), type);
			OpenGLContext::callback(data);
		}
	}*/


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
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_CULL_FACE);
		//glEnable(GL_DEBUG_OUTPUT);
		//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		//glDebugMessageCallback(OpenGLContext::OnError, this);
	}

	void OpenGLContext::ClearFrame(Color clearColor) const
	{
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLContext::SetViewport(int x, int y, int w, int h) const
	{
		glViewport(x, y, w, h);
	}

	void OpenGLContext::DrawIndexed(VertexArray* array) const
	{
		array->EnableAll();
		glDrawElements(GL_TRIANGLES, array->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLContext::SetErrorListener(const ErrorCallback& callback)
	{
		OpenGLContext::callback = callback;
		HZR_CORE_INFO("Added OpenGL error callback");
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