
#include "OpenGLRenderCommandBuffer.h"
#include "OpenGLSwapchain.h"
#include "OpenGLCore.h"

#include "Backend/Core/Renderer.h"

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
	void OpenGLRenderCommandBuffer::BeginRenderPass(Ref<RenderPass> renderPass, bool explicitClear)
	{
		Ref<OpenGLRenderPass> instance = renderPass.As<OpenGLRenderPass>();
		Renderer::Submit([instance, explicitClear]() mutable {
			instance->GetSpecs().TargetFrameBuffer.As<OpenGLFrameBuffer>()->Bind_RT();
			});
	}
	void OpenGLRenderCommandBuffer::EndRenderPass()
	{

	}
}