#include "hzrpch.h"
#include "RenderEngine.h"
#include "HRenderer.h"
#include "Mesh/MeshAssetLoader.h"
#include "Environment/EnvironmentAssetLoader.h"
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
		AssetManager::RegisterLoader<EnvironmentAssetLoader>(AssetType::EnvironmentMap);

		m_Window = Window::Create(createInfo);
		m_Window->Show();

		UniformBufferCreateInfo cameraUBO = {};
		cameraUBO.Name = "Camera";
		cameraUBO.Binding = 0;
		cameraUBO.Size = sizeof(CameraData);
		cameraUBO.Usage = BufferUsage::DynamicDraw;

		m_CameraUniformBuffer = UniformBuffer::Create(&cameraUBO);

		UniformBufferCreateInfo lightUBO = {};
		lightUBO.Name = "Lights";
		lightUBO.Binding = 1;
		lightUBO.Size = sizeof(LightingData);
		lightUBO.Usage = BufferUsage::DynamicDraw;

		m_LightUniformBuffer = UniformBuffer::Create(&lightUBO);

		UniformBufferCreateInfo modelUBO = {};
		modelUBO.Name = "Model";
		modelUBO.Binding = 2;
		modelUBO.Size = sizeof(glm::mat4);
		modelUBO.Usage = BufferUsage::DynamicDraw;

		m_ModelUniformBuffer = UniformBuffer::Create(&modelUBO);

		uint32_t data = 0xFFFFFFFF;

		Image2DCreateInfo info = {};
		info.Width = 1;
		info.Height = 1;
		info.Mips = 1;
		info.Data = Buffer::Copy(&data, sizeof(uint32_t));
		info.Format = ImageFormat::RGBA;

		Ref<Hazard::Image2D> imageAsset = HazardRenderer::Image2D::Create(&info);

		FrameBufferCreateInfo deferredFbo = {};
		deferredFbo.DebugName = "DeferredFBO";
		deferredFbo.SwapChainTarget = false;
		deferredFbo.Width = 1280;
		deferredFbo.Height = 720;
		deferredFbo.ClearColor = { 0.0,0.0,0.0,1.0 };
		deferredFbo.AttachmentCount = 4;
		deferredFbo.Attachments = { { ImageFormat::RGBA16F, ImageFormat::RGBA16F, ImageFormat::RGBA, ImageFormat::Depth } };

		m_DeferredFrameBuffer = FrameBuffer::Create(&deferredFbo);

		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = deferredFbo.DebugName;
		renderPassInfo.pTargetFrameBuffer = m_DeferredFrameBuffer;

		m_DeferredRenderPass = RenderPass::Create(&renderPassInfo);


		BufferLayout layout = { {} };

		PipelineSpecification compositeInfo = {};
		compositeInfo.DebugName = "CompositePipeline";
		compositeInfo.pTargetRenderPass = m_DeferredRenderPass.Raw();
		compositeInfo.ShaderPath = "Shaders/Composite/SceneComposite.glsl";
		compositeInfo.DrawType = DrawType::Fill;
		compositeInfo.Usage = PipelineUsage::GraphicsBit;
		compositeInfo.CullMode = CullMode::None;
		compositeInfo.IsShared = false;
		compositeInfo.pBufferLayout = &layout;

		m_CompositePipeline = Pipeline::Create(&compositeInfo);

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
		renderer->OnRenderExtra();

		m_LineRenderer.EndScene();
		m_QuadRenderer.EndScene();
	}
	void RenderEngine::GeometryPass(const Ref<RenderCommandBuffer>& cmdBuffer)
	{
		HZR_PROFILE_FUNCTION();

		m_ModelUniformBuffer->Bind(cmdBuffer);

		auto& drawList = GetDrawList();
		for (auto& [pipeline, list] : drawList.MeshList) 
		{
			pipeline->Bind(cmdBuffer);
			for (auto& mesh : list) 
			{
				if (mesh.Count == 0) continue;
				struct ModelData 
				{
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
	void RenderEngine::CompositePass(Ref<RenderCommandBuffer> commandBuffer)
	{
		HZR_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < m_DeferredFrameBuffer->GetColorAttachmentCount(); i++) 
		{
			auto& image = m_DeferredFrameBuffer->GetImage(i);
			image->Bind(i);
		}
		m_CompositePipeline->Bind(commandBuffer);
		m_CompositePipeline->DrawArrays(commandBuffer, 6);
	}
	void RenderEngine::PrepareLights()
	{
		auto& drawList = GetDrawList();
		LightingData data = {};
		data.DirectionLightCount = Math::Max<uint32_t>(drawList.DirectionalLights.size(), 16);
		
		for (size_t i = 0; i < data.DirectionLightCount; i++)
		{
			const DirectionalLightSource& light = drawList.DirectionalLights[i];
			DirectionalLight& l = data.Lights[i];
			l.Direction = glm::vec4(light.Direction, 1.0);
			l.Color = { light.Color, light.Intensity };
		}
		m_LightUniformBuffer->SetData(&data, sizeof(LightingData));
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
		m_CameraUniformBuffer->Bind(cmdBuffer);
		m_LightUniformBuffer->Bind(cmdBuffer);

		m_CurrentDrawContext = 0;

		for (auto& renderer : m_DrawList)
		{
			//Prerender will submit all meshes to rendering for a given world
			PreRender(renderer.WorldRenderer);
			ShadowPass();
			PrepareLights();

			//Bind G buffer attachments
			m_DeferredFrameBuffer->Bind();

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
				context->BeginRenderPass(cmdBuffer, m_DeferredRenderPass);
				GeometryPass(cmdBuffer);
				context->EndRenderPass(cmdBuffer);

				context->BeginRenderPass(cmdBuffer, cameraData.RenderPass);

				m_CompositePipeline->SetRenderPass(cameraData.RenderPass);
				CompositePass(cmdBuffer);
				context->EndRenderPass(cmdBuffer);

				break;
			}

			m_CurrentDrawContext++;
		}
	}
	void RenderEngine::PostRender()
	{
		//Clear cameras
		for (auto& renderer : m_DrawList) {
			renderer.WorldRenderer->m_CameraData.clear();
			renderer.WorldRenderer->m_RendererExtraCalls.clear();
		}
		m_DrawList.clear();
	}
}
