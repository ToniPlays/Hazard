#include "hzrpch.h"
#include "RenderEngine.h"
#include "HazardRenderer.h"
#include "HRenderer.h"

namespace Hazard
{
	using namespace HazardRenderer;

	RenderEngine::RenderEngine(HazardRenderer::HazardRendererCreateInfo* createInfo) : Module("RenderEngine")
	{
		m_Window = Window::Create(createInfo);
		m_Window->Show();
		HRenderer::s_Engine = this;
	}
	void RenderEngine::CullingPass()
	{
		HZR_PROFILE_FUNCTION();
	}
	void RenderEngine::GeometryPass()
	{
		HZR_PROFILE_FUNCTION();
	}
	void RenderEngine::ShadowPass()
	{
		HZR_PROFILE_FUNCTION();
	}
	void RenderEngine::CompositePass()
	{
		HZR_PROFILE_FUNCTION();
		GraphicsContext* context = m_Window->GetContext();
		Ref<RenderCommandBuffer> cmdBuffer = context->GetSwapchain()->GetSwapchainBuffer();

		for (auto& camera : m_DrawList.RenderingCameras)
		{
			context->BeginRenderPass(cmdBuffer, camera.RenderPass);
			context->EndRenderPass(cmdBuffer);
		}
	}
	void RenderEngine::Update()
	{
	}
	void RenderEngine::Render()
	{
		HZR_PROFILE_FUNCTION();
		CullingPass();
		GeometryPass();
		ShadowPass();
		CompositePass();

	}
	void RenderEngine::PostRender()
	{
		m_DrawList.RenderingCameras.clear();
	}
}
