
#include "OpenGLContext.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/Window.h"
#include "OpenGLSwapchain.h"

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
			s_Instance = this;
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
			m_Window = window;

			glfwMakeContextCurrent((GLFWwindow*)m_Window->GetNativeWindow());

			HZR_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLFW OpenGL context");

			m_PhysicalDevice = new OpenGLPhysicalDevice();
			m_Swapchain = Ref<OpenGLSwapchain>::Create(this, info->pTargetFrameBuffer);
			m_Swapchain->Create(m_Window->GetWidth(), m_Window->GetHeight(), info->VSync);


			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			if (!info->Logging) return;

			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OnDebugMessage, nullptr);

			Window::SendDebugMessage({ Severity::Info, "Debugging enabled" });
		}

		void OpenGLContext::BeginFrame()
		{
			m_Swapchain->BeginFrame();
			BeginRenderPass(m_Swapchain->GetSwapchainBuffer(), m_Swapchain->GetRenderPass());
		}

		void OpenGLContext::Present()
		{
			EndRenderPass(m_Swapchain->GetSwapchainBuffer());
			m_Swapchain->Present();
		}

		void OpenGLContext::BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass)
		{
			renderPass->GetSpecs().TargetFrameBuffer->Bind();
		}

		void OpenGLContext::EndRenderPass(Ref<RenderCommandBuffer> buffer)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
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
}
#endif
