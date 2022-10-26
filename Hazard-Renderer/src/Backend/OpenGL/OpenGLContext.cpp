
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
		Renderer::Init(this);
	}
	OpenGLContext::~OpenGLContext()
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_DEBUG_OUTPUT);
		glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	void OpenGLContext::Init(Window* window, HazardRendererCreateInfo* info)
	{
		HZR_PROFILE_FUNCTION();
		m_Window = window;

		glfwMakeContextCurrent((GLFWwindow*)m_Window->GetNativeWindow());

		HZR_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLFW OpenGL context");

		m_PhysicalDevice = new OpenGLPhysicalDevice();
		m_Swapchain = Ref<OpenGLSwapchain>::Create(this, info->pTargetFrameBuffer);
		m_Swapchain->Create(m_Window->GetWidth(), m_Window->GetHeight(), info->VSync);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		if (!info->Logging) return;

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OnDebugMessage, nullptr);

		Window::SendDebugMessage({ Severity::Info, "Debugging enabled" });
	}

	void OpenGLContext::BeginFrame()
	{
		HZR_PROFILE_FUNCTION();
		m_Swapchain->BeginFrame();
	}
	void OpenGLContext::Present()
	{
		HZR_PROFILE_FUNCTION();
		m_Swapchain->Present();
	}
	glm::vec4 OpenGLContext::GetClearColor()
	{
		return m_Swapchain->GetRenderTarget()->GetSpecification().ClearColor;
	}
	void OpenGLContext::SetClearColor(const glm::vec4& color)
	{
		m_Swapchain->GetRenderTarget()->GetSpecification().ClearColor = color;
	}
}
#endif
