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
		m_QuadRenderer.CreateResources();

		UniformBufferCreateInfo cameraUBO = {};
		cameraUBO.Name = "Camera";
		cameraUBO.Binding = 0;
		cameraUBO.Size = sizeof(CameraData);
		cameraUBO.Usage = BufferUsage::DynamicDraw;

		m_CameraUniformBuffer = UniformBuffer::Create(&cameraUBO);
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

	}
	void RenderEngine::Update()
	{
		m_QuadRenderer.BeginScene();
	}
	void RenderEngine::Render()
	{
		HZR_PROFILE_FUNCTION();
		CullingPass();
		GeometryPass();
		ShadowPass();
		CompositePass();

		GraphicsContext* context = m_Window->GetContext();
		Ref<RenderCommandBuffer> cmdBuffer = context->GetSwapchain()->GetSwapchainBuffer();

		for (auto& camera : m_DrawList.RenderingCameras)
		{
			//Prepare camera data
			CameraData cam = {};
			cam.ViewProjection = camera.ViewProjection;
			cam.Projection = glm::mat4(1.0f);
			cam.View = glm::mat4(1.0f);
			cam.Position = { camera.Position, 1.0f };

			uint32_t size = sizeof(CameraData);

			m_CameraUniformBuffer->SetData(&cam, sizeof(CameraData));

			context->BeginRenderPass(cmdBuffer, camera.RenderPass);
			m_QuadRenderer.EndScene();
			context->EndRenderPass(cmdBuffer);
		}
	}
	void RenderEngine::PostRender()
	{
		m_DrawList.RenderingCameras.clear();
		m_DrawList.Environment.clear();
		m_DrawList.LightSource.clear();
	}
}
