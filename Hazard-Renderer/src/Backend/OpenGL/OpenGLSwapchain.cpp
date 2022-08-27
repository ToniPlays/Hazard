

#include "OpenGLSwapchain.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace HazardRenderer::OpenGL
{
	OpenGLSwapchain::OpenGLSwapchain(OpenGLContext* context, FrameBufferCreateInfo* targetInfo) : m_Context(context)
	{
		if (targetInfo == nullptr) {
			//Create default target

			FrameBufferCreateInfo frameBufferInfo = {};
			frameBufferInfo.DebugName = "ScreenFBO";
			frameBufferInfo.SwapChainTarget = true;
			frameBufferInfo.AttachmentCount = 2;
			frameBufferInfo.Attachments = { { ImageFormat::RGBA }, { ImageFormat::Depth } };

			m_FrameBuffer = FrameBuffer::Create(&frameBufferInfo);

			RenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.DebugName = "ScreenTarget";
			renderPassInfo.pTargetFrameBuffer = m_FrameBuffer;

			m_RenderPass = RenderPass::Create(&renderPassInfo);
		}
		else 
		{
			m_FrameBuffer = FrameBuffer::Create(targetInfo);

			RenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.DebugName = "ScreenTarget";
			renderPassInfo.pTargetFrameBuffer = m_FrameBuffer;
			
			m_RenderPass = RenderPass::Create(&renderPassInfo);
		}
	}
	void OpenGLSwapchain::Create(uint32_t width, uint32_t height, bool vSync)
	{
		m_Width = width;
		m_Height = height;
		m_Vsync = vSync;

		if (!m_RenderCommandBuffer) 
		{
			m_RenderCommandBuffer = RenderCommandBuffer::CreateFromSwapchain("OpenGLSwapchain");
		}
	}
	void OpenGLSwapchain::Resize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
	}
	void OpenGLSwapchain::BeginFrame()
	{
		HZR_PROFILE_FUNCTION();
		m_FrameIndex = ++m_FrameIndex % 2;
		m_RenderCommandBuffer->Begin();
	}

	void OpenGLSwapchain::Present()
	{
		HZR_PROFILE_FUNCTION();
		glfwSwapInterval(m_Vsync ? 1 : 0);
		Window* window = m_Context->GetWindow();
		glfwSwapBuffers((GLFWwindow*)window->GetNativeWindow());
	}
}
#endif
