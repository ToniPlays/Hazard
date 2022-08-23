#include "hzrpch.h"
#include "RenderEngine.h"
#include "HRenderer.h"
#include "Mesh/MeshAssetLoader.h"
#include "ImageAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardRenderer;

	RenderEngine::RenderEngine(HazardRenderer::HazardRendererCreateInfo* createInfo) : Module("RenderEngine")
	{
		HRenderer::s_Engine = this;
		AssetManager::RegisterLoader<MeshAssetLoader>(AssetType::Mesh);
		AssetManager::RegisterLoader<ImageAssetLoader>(AssetType::Image);

		m_Window = Window::Create(createInfo);
		m_Window->Show();

		UniformBufferCreateInfo cameraUBO = {};
		cameraUBO.Name = "Camera";
		cameraUBO.Binding = 0;
		cameraUBO.Size = sizeof(CameraData);
		cameraUBO.Usage = BufferUsage::DynamicDraw;

		m_CameraUniformBuffer = UniformBuffer::Create(&cameraUBO);

		UniformBufferCreateInfo modelUBO = {};
		modelUBO.Name = "Model";
		modelUBO.Binding = 1;
		modelUBO.Size = sizeof(glm::mat4);
		cameraUBO.Usage = BufferUsage::DynamicDraw;

		m_ModelUniformBuffer = UniformBuffer::Create(&modelUBO);

		uint32_t data = 0xFFFFFFFF;

		Image2DCreateInfo info = {};
		info.Width = 1;
		info.Height = 1;
		info.Mips = 1;
		info.Data = Buffer::Copy(&data, sizeof(uint32_t));
		info.Format = ImageFormat::RGBA;

		Ref<Hazard::Image2D> imageAsset = HazardRenderer::Image2D::Create(&info);

		m_WhiteTexture = Ref<Texture2D>::Create(imageAsset);
		m_QuadRenderer.Init();
		m_QuadRenderer.CreateResources();

		m_LineRenderer.Init();
		m_LineRenderer.CreateResources();
	}
	void RenderEngine::PreRender(Ref<WorldRenderer> renderer)
	{
		m_LineRenderer.BeginScene();
		m_QuadRenderer.BeginScene();

		renderer->Render();

		m_LineRenderer.EndScene();
		m_QuadRenderer.EndScene();
	}
	void RenderEngine::CullingPass()
	{
		HZR_PROFILE_FUNCTION();
	}
	void RenderEngine::GeometryPass(const Ref<RenderCommandBuffer>& cmdBuffer)
	{
		HZR_PROFILE_FUNCTION();

		auto& drawList = GetDrawList();
		for (auto& [pipeline, list] : drawList.MeshList) 
		{
			pipeline->Bind(cmdBuffer);
			for (auto& mesh : list) 
			{
				if (mesh.Count == 0) continue;
				struct ModelData {
					glm::mat4 transform;
				};

				ModelData data;
				data.transform = mesh.Transform;

				m_ModelUniformBuffer->SetData(&data, sizeof(ModelData));
				mesh.VertexBuffer->Bind(cmdBuffer);

				if (mesh.IndexBuffer) 
				{
					mesh.IndexBuffer->Bind(cmdBuffer);
					pipeline->Draw(cmdBuffer, mesh.Count);
					drawList.Stats.DrawCalls++;
				}
				else
				{
					pipeline->DrawArrays(cmdBuffer, mesh.Count);
					drawList.Stats.DrawCalls++;
				}
			}
		}
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
		Input::Update();
	}
	void RenderEngine::Render()
	{
		HZR_PROFILE_FUNCTION();

		GraphicsContext* context = m_Window->GetContext();
		Ref<RenderCommandBuffer> cmdBuffer = context->GetSwapchain()->GetSwapchainBuffer();

		m_CurrentDrawContext = 0;
		for (auto& renderer : m_DrawList)
		{
			//Prerender will submit all meshes to rendering for a given world
			PreRender(renderer.WorldRenderer);

			for (auto& cameraData : renderer.WorldRenderer->GetCameraData())
			{
				if (!cameraData.IsValid()) continue;

				CameraData cam = {};
				cam.ViewProjection = cameraData.ViewProjection;
				cam.Projection = cameraData.Projection;
				cam.View = cameraData.View;
				cam.Position = { cameraData.Position, 1.0f };

				m_CameraUniformBuffer->SetData(&cam, sizeof(CameraData));

				//Actual rendering
				context->BeginRenderPass(cmdBuffer, cameraData.RenderPass);

				GeometryPass(cmdBuffer);

				context->EndRenderPass(cmdBuffer);
			}

			m_CurrentDrawContext++;
		}
	}
	void RenderEngine::PostRender()
	{
		//Clear cameras
		for (auto& renderer : m_DrawList) {
			renderer.WorldRenderer->m_CameraData.clear();
		}
		m_DrawList.clear();
	}
}
