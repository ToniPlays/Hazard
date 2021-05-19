#pragma once

#include <hzrpch.h>
#include "OpenGLContext.h"
#include "OpenGLUtils.h"

#include <glad/glad.h>

namespace Hazard::Rendering {

	namespace OpenGL
	{
		ErrorCallback OpenGLContext::s_Callback;

		void APIENTRY OnDebugMessage(GLenum source, GLenum type, unsigned int id, GLenum severity,
			GLsizei length, const char* message, const void* userParam) {

			if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

			HZR_CORE_ERROR("[OpenGL]: {0}", message);

			OpenGLContext::SendDebugMessage(message, OpenGLUtils::GluintToString(severity));
		}

		OpenGLContext::OpenGLContext(WindowProps* props)
		{
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		}

		OpenGLContext::~OpenGLContext()
		{
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_DEBUG_OUTPUT);
			glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}

		void OpenGLContext::Init(Window* window, ApplicationCreateInfo* appInfo)
		{
			m_Window = (GLFWwindow*)window->GetNativeWindow();

			glfwMakeContextCurrent(m_Window);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				HZR_THROW("Unable to init OpenGL GLFW context");
			};

			Enable(CullFace);
			glCullFace(GL_BACK);
			Enable(Depth);
			Enable(Blend);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if (!appInfo->logging) return;

			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OnDebugMessage, nullptr);
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

		void OpenGLContext::SetDepthTest(DepthFunc type) const
		{
			glDepthFunc(GL_NEVER + type);
		}

		void OpenGLContext::Enable(Function fn) const
		{
			switch (fn)
			{
			case Multisample:
				glEnable(GL_MULTISAMPLE);
				break;
			case CullFace:
				glEnable(GL_CULL_FACE);
				break;
			case Depth:
				glEnable(GL_DEPTH_TEST);
				break;
			case Blend:
				glEnable(GL_BLEND);
				break;
			}
		}

		void OpenGLContext::Disable(Function fn) const
		{
			switch (fn)
			{
			case Hazard::Rendering::Multisample:
				glDisable(GL_MULTISAMPLE);
				break;
			case CullFace:
				glDisable(GL_CULL_FACE);
				break;
			case Depth:
				glDisable(GL_DEPTH_TEST);
				break;
			case Blend:
				glDisable(GL_BLEND);
				break;
			}
		}

		void OpenGLContext::DrawIndexed(VertexArray& array, uint32_t size) const
		{
			array.EnableAll();
			glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
		}

		void OpenGLContext::SetErrorListener(const ErrorCallback& callback)
		{
			OpenGLContext::s_Callback = callback;
		}

		DeviceSpec OpenGLContext::GetDeviceSpec() const
		{
			DeviceSpec spec;
			std::stringstream ss;
			GLint major, minor, slots;

			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &slots);

			ss << "OpenGL ";
			ss << major << "." << minor;
			spec.renderer = ss.str();
			ss.str("");

			ss << glGetString(GL_RENDERER);

			spec.name = ss.str();
			spec.textureSlots = major;
			return spec;
		}
		void OpenGLContext::SendDebugMessage(const char* message, const char* code)
		{
			if (!s_Callback) return;
			ErrorData data(("[OpenGL]: " + std::string(message)).c_str(), code);
			s_Callback(data);
		}
	}
}