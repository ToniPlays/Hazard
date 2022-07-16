
#include "OpenGLContext.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/Renderer.h"
#include "Backend/Core/Window.h"
#include "OpenGLSwapchain.h"
#include "OpenGLFramebuffer.h"

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
		Renderer::Init();
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
		Ref<OpenGLSwapchain> swapchain = m_Swapchain;
		Renderer::Submit([swapchain]() mutable {
			swapchain->BeginFrame();
			});
	}

	void OpenGLContext::Present()
	{
		Ref<OpenGLSwapchain> swapchain = m_Swapchain;
		OpenGLContext* instance = this;
		Renderer::Submit([instance, swapchain]() mutable {
			swapchain->Present();
			});
	}

	void OpenGLContext::BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass)
	{
		Renderer::Submit([renderPass]() mutable {
			HZR_PROFILE_FUNCTION("OpenGLContext::BeginRenderPass_RT()");
			renderPass->GetSpecs().TargetFrameBuffer.As<OpenGLFrameBuffer>()->Bind_RT();
			});
	}

	void OpenGLContext::EndRenderPass(Ref<RenderCommandBuffer> buffer)
	{
		Renderer::Submit([]() mutable {
			HZR_PROFILE_FUNCTION("OpenGLContext::EndRenderPass_RT()");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			});
	}
	glm::vec4 OpenGLContext::GetClearColor()
	{
		return m_Swapchain->GetRenderTarget()->GetSpecification().ClearColor;
	}
	void OpenGLContext::SetClearColor(const glm::vec4& color)
	{
		m_Swapchain->GetRenderTarget()->GetSpecification().ClearColor = color;
	}
	void OpenGLContext::SetViewport(int x, int y, int w, int h)
	{
		Renderer::Submit([x, y, w, h]() mutable {
			HZR_PROFILE_FUNCTION("OpenGLContext::SetViewport_RT()");
			glViewport(x, y, w, h);
			});
	}
}
#endif
