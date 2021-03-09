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
			HZR_ASSERT(false, "Unable to init GLFW context");
			return;
		};
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_DEBUG_OUTPUT);
		//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		//glDebugMessageCallback(OpenGLContext::OnError, this);
	}

	void OpenGLContext::ClearFrame(glm::vec4 clearColor) const
	{
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLContext::SetViewport(int x, int y, int w, int h) const
	{
		glViewport(x, y, w, h);
	}

	void OpenGLContext::DrawIndexed(VertexArray* array, uint32_t size) const
	{
		array->EnableAll();
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLContext::SetErrorListener(const ErrorCallback& callback)
	{
		OpenGLContext::callback = callback;
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
	std::string OpenGLContext::GetDevice() const
	{
		std::stringstream ss;
		ss << glGetString(GL_RENDERER);
		return ss.str();
	}
}