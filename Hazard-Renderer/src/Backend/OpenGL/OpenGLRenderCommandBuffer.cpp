
#include "OpenGLRenderCommandBuffer.h"
#include "OpenGLSwapchain.h"

namespace HazardRenderer::OpenGL 
{
	OpenGLRenderCommandBuffer::OpenGLRenderCommandBuffer(uint32_t size, const std::string& name)
	{
		m_Swapchain = OpenGLContext::GetInstance().GetSwapchain();
	}
	OpenGLRenderCommandBuffer::OpenGLRenderCommandBuffer(const std::string& name, bool swapchain)
	{
		m_Swapchain = OpenGLContext::GetInstance().GetSwapchain();
	}
	void OpenGLRenderCommandBuffer::Begin()
	{
		m_FrameIndex = m_Swapchain.As<OpenGLSwapchain>()->GetFrameIndex();
	}
}