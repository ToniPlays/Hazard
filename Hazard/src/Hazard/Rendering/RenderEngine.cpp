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

		if (!createInfo->UseResources || true) 
		{
			SetActive(false);
			return;
		}

		uint32_t data = 0xFFFFFFFF;

		Image2DCreateInfo info = {};
		info.DebugName = "WhiteTexture";
		info.Width = 1;
		info.Height = 1;
		info.Mips = 1;
		info.Data = Buffer::Copy(&data, sizeof(uint32_t));
		info.Format = ImageFormat::RGBA;

		Ref<Hazard::Image2DAsset> imageAsset = Ref<Hazard::Image2DAsset>::Create(&info);

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

		m_WhiteTexture = Ref<Texture2DAsset>::Create(imageAsset);
		m_QuadRenderer.Init();
		m_QuadRenderer.CreateResources();

		m_LineRenderer.Init();
		m_LineRenderer.CreateResources();

		m_Resources = new RenderResources();
		m_Resources->Initialize(m_DeferredRenderPass);

	}
	void RenderEngine::PreRender(Ref<WorldRenderer> renderer)
	{
		HZR_PROFILE_FUNCTION();
		m_LineRenderer.BeginScene();
		m_QuadRenderer.BeginScene();

		renderer->Render();
		renderer->OnRenderExtra();

		m_LineRenderer.EndScene();
		m_QuadRenderer.EndScene();
	}
	void RenderEngine::GeometryPass(const Ref<RenderCommandBuffer>& cmdBuffer)
	{
		/*
		HZR_PROFILE_FUNCTION();
		auto& drawList = GetDrawList();
		m_Resources->ModelUniformBuffer->Bind(cmdBuffer);

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

				m_Resources->ModelUniformBuffer->SetData(&data, sizeof(ModelData));
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
		}*/
	}
	void RenderEngine::ShadowPass(Ref<RenderCommandBuffer> commandBuffer)
	{
		HZR_PROFILE_FUNCTION();
	}
	void RenderEngine::CompositePass(Ref<RenderCommandBuffer> commandBuffer)
	{
		HZR_PROFILE_FUNCTION();
		/*
		const auto& drawList = GetDrawList();

		//Skybox
		if (drawList.Environment.size() > 0)
		{
			for (auto& [map, data] : drawList.Environment)
			{
				map->RadianceMap->Bind(0);
				m_Resources->SkyboxPipeline->Bind(commandBuffer);
				m_Resources->SkyboxPipeline->DrawArrays(commandBuffer, 6);
				break;
			}
		}

		for (uint32_t i = 0; i < m_DeferredFrameBuffer->GetColorAttachmentCount(); i++) 
		{
			auto& image = m_DeferredFrameBuffer->GetImage(i);
			image->Bind(i);
		}
		m_Resources->CompositePipeline->Bind(commandBuffer);
		m_Resources->CompositePipeline->DrawArrays(commandBuffer, 6);
		*/
	}
	void RenderEngine::PrepareLights()
	{
		/*
		HZR_PROFILE_FUNCTION();
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
		m_Resources->LightUniformBuffer->SetData(&data, sizeof(LightingData));
		*/
	}
	void RenderEngine::ClearDrawLists()
	{
		HZR_PROFILE_FUNCTION();
		//Clear cameras
		for (auto& renderer : m_DrawList) {
			renderer.WorldRenderer->m_CameraData.clear();
			renderer.WorldRenderer->m_RendererExtraCalls.clear();
		}
		m_DrawList.clear();
	}
	void RenderEngine::Update()
	{
		HZR_PROFILE_FUNCTION();
		Input::Update();
		ClearDrawLists();
	}
	void RenderEngine::Render()
	{
		HZR_PROFILE_FUNCTION();
		/*
		m_Resources->CameraUniformBuffer->Bind(cmdBuffer);
		m_Resources->LightUniformBuffer->Bind(cmdBuffer);

		m_CurrentDrawContext = 0;

		for (auto& renderer : m_DrawList)
		{
			//Prerender will submit all meshes to rendering for a given world
			PreRender(renderer.WorldRenderer);
			ShadowPass(cmdBuffer);
			PrepareLights();

			//Bind G buffer attachments
			m_DeferredFrameBuffer->Bind();

			for (auto& cameraData : renderer.WorldRenderer->GetCameraData())
			{
				if (!cameraData.IsValid()) continue;

				m_DeferredFrameBuffer->Resize(cameraData.Width, cameraData.Height);
				const glm::mat4 inverseView = glm::inverse(cameraData.View);

				CameraData cam = {};
				cam.ViewProjection = cameraData.Projection * inverseView;
				cam.Projection = cameraData.Projection;
				cam.View = inverseView;
				cam.InverseViewProjection = cameraData.View * glm::inverse(cameraData.Projection);
				cam.Position = { cameraData.Position, 1.0f };

				m_Resources->CameraUniformBuffer->SetData(&cam, sizeof(CameraData));

				//Actual rendering
				context->BeginRenderPass(cmdBuffer, m_DeferredRenderPass);
				GeometryPass(cmdBuffer);
				context->EndRenderPass(cmdBuffer);

				m_Resources->CompositePipeline->SetRenderPass(cameraData.RenderPass);
				context->BeginRenderPass(cmdBuffer, cameraData.RenderPass);

				CompositePass(cmdBuffer);
				context->EndRenderPass(cmdBuffer);
				break;
			}
			m_CurrentDrawContext++;
		}
		*/
	}
}
