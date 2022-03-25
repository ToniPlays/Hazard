#ifdef HZR_INCLUDE_OPENGL

#include "OpenGLContext.h"
#include "Core/Window.h"
#include "OpenGLUtils.h"
#include <glad/glad.h>

namespace HazardRenderer::OpenGL {


		void APIENTRY OnDebugMessage(GLenum source, GLenum type, unsigned int id, GLenum severity,
			GLsizei length, const char* message, const void* userParam) {

			if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) 
				return;
			Window::SendDebugMessage({ OpenGLUtils::GLuintToSeverity(severity), message });
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

		void OpenGLContext::Init(Window* window, HazardRendererCreateInfo* info)
		{
			m_Window = (GLFWwindow*)window->GetNativeWindow();

			glfwMakeContextCurrent(m_Window);

			ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLFW OpenGL context");

			m_PhysicalDevice = new OpenGLPhysicalDevice();

			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			if (!info->Logging) return;

			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			Window::SendDebugMessage({ Severity::Info, "Debugging enabled" });
		}

		void OpenGLContext::BeginFrame()
		{
			glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void OpenGLContext::Present()
		{
			glfwSwapBuffers(m_Window);
		}

//		void OpenGLContext::BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass)
//		{
//			renderPass->GetSpecs().TargetFrameBuffer->Bind();
//		}
//
//		void OpenGLContext::EndRenderPass(Ref<RenderCommandBuffer> buffer)
//		{
//			glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		}
//
//		void OpenGLContext::SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth)
//		{
//			glLineWidth(lineWidth);
//		}
//
		void OpenGLContext::SetViewport(int x, int y, int w, int h)
		{
			glViewport(x, y, w, h);
		}
//
//		void OpenGLContext::SetErrorListener(const ErrorCallback& callback)
//		{
//			OpenGLContext::s_Callback = callback;
//		}
//
//		PhysicalDevice& OpenGLContext::GetPhysicalDevice() const
//		{
//			return *m_PhysicalDevice;
//		}
//		void OpenGLContext::SendDebugMessage(const char* message, const char* code)
//		{
//			if (!s_Callback) 
//				return;
//			ErrorData data(("[OpenGL]: " + std::string(message)).c_str(), code);
//			s_Callback(data);
//		}
}
#endif
