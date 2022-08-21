#include "hzrpch.h"
#include "RenderEngine.h"
#include "HazardRenderer.h"
#include "HRenderer.h"
#include "Mesh/MeshAssetLoader.h"
#include "ImageAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardRenderer;

	RenderEngine::RenderEngine(HazardRenderer::HazardRendererCreateInfo* createInfo) : Module("RenderEngine")
	{
		m_Window = Window::Create(createInfo);
		m_Window->Show();
		HRenderer::s_Engine = this;
		

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

		AssetManager::RegisterLoader<MeshAssetLoader>(AssetType::Mesh);
		AssetManager::RegisterLoader<ImageAssetLoader>(AssetType::Image);

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
	void RenderEngine::CullingPass()
	{
		HZR_PROFILE_FUNCTION();
	}
	void RenderEngine::GeometryPass()
	{
		HZR_PROFILE_FUNCTION();
		m_QuadRenderer.EndScene();
		m_LineRenderer.EndScene();

		auto& cmdBuffer = m_Window->GetSwapchain()->GetSwapchainBuffer();
		m_ModelUniformBuffer->Bind(cmdBuffer);

		for (auto& [pipeline, dataList] : m_DrawList.Pipelines) {
			pipeline->Bind(cmdBuffer);
			for (auto& data : dataList) {
				pipeline->DrawArrays(cmdBuffer, data.Count);
			}
		}

		for (auto& [pipeline, meshList] : m_DrawList.Meshes) {
			pipeline->Bind(cmdBuffer);

			for (auto& rawMesh : meshList)
			{
				struct ModelData {
					glm::mat4 transform;
				};

				ModelData data;
				data.transform = rawMesh.Transform;

				m_ModelUniformBuffer->SetData(&data, sizeof(ModelData));
				rawMesh.VertexBuffer->Bind(cmdBuffer);
				if (rawMesh.IndexBuffer) {
					rawMesh.IndexBuffer->Bind(cmdBuffer);
					pipeline->Draw(cmdBuffer, rawMesh.Count);
				}
				else 
				{
					pipeline->DrawArrays(cmdBuffer, rawMesh.Count);
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
		m_QuadRenderer.BeginScene();
		m_LineRenderer.BeginScene();
	}
	void RenderEngine::Render()
	{
		HZR_PROFILE_FUNCTION();
		CullingPass();
		ShadowPass();

		GraphicsContext* context = m_Window->GetContext();
		Ref<RenderCommandBuffer> cmdBuffer = context->GetSwapchain()->GetSwapchainBuffer();

		for (auto& camera : m_DrawList.RenderingCameras)
		{
			//Prepare camera data
			CameraData cam = {};
			cam.ViewProjection = camera.ViewProjection;
			cam.Projection = camera.Projection;
			cam.View = camera.View;
			cam.Position = { camera.Position, 1.0f };

			uint32_t size = sizeof(CameraData);

			m_CameraUniformBuffer->SetData(&cam, sizeof(CameraData));
			context->BeginRenderPass(cmdBuffer, camera.RenderPass);

			GeometryPass();
			CompositePass();

			context->EndRenderPass(cmdBuffer);
		}
	}
	void RenderEngine::PostRender()
	{
		m_DrawList.RenderingCameras.clear();
		m_DrawList.Environment.clear();
		m_DrawList.LightSource.clear();
		m_DrawList.Meshes.clear();
		m_DrawList.Pipelines.clear();
	}
}
