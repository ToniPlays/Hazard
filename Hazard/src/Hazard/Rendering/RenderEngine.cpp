#include "hzrpch.h"
#include "RenderEngine.h"
#include "HRenderer.h"
#include "Mesh/MeshAssetLoader.h"
#include "Environment/EnvironmentAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/Texture2D.h"
#include "Hazard/Core/Application.h"

namespace Hazard
{
	RenderEngine::RenderEngine(RendererCreateInfo* createInfo) : Module("RenderEngine")
	{
		using namespace HazardRenderer;
		HRenderer::s_Engine = this;
		AssetManager::RegisterLoader<MeshAssetLoader>(AssetType::Mesh);
		AssetManager::RegisterLoader<EnvironmentAssetLoader>(AssetType::EnvironmentMap);

		uint32_t data = 0xFFFFFFFF;

		Image2DCreateInfo info = {};
		info.DebugName = "WhiteTexture";
		info.Width = 1;
		info.Height = 1;
		info.Mips = 1;
		info.Data = Buffer::Copy(&data, sizeof(uint32_t));
		info.Format = ImageFormat::RGBA;
		info.Usage = ImageUsage::Texture;

		m_WhiteTexture = Ref<Texture2DAsset>::Create(Ref<Hazard::Image2DAsset>::Create(&info));

		FrameBufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.DebugName = "RenderEngine";
		frameBufferInfo.AttachmentCount = 4;
		frameBufferInfo.Attachments = { { ImageFormat::RGBA, ImageFormat::Depth } };
		frameBufferInfo.ClearOnLoad = true;
		frameBufferInfo.Width = 1920;
		frameBufferInfo.Height = 1080;
		frameBufferInfo.SwapChainTarget = false;

		m_FrameBuffer = FrameBuffer::Create(&frameBufferInfo);

		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = "RenderEngine";
		renderPassInfo.pTargetFrameBuffer = m_FrameBuffer;

		m_RenderPass = RenderPass::Create(&renderPassInfo);

		m_QuadRenderer.Init();
		m_QuadRenderer.CreateResources(m_RenderPass);
		m_LineRenderer.Init();
		m_LineRenderer.CreateResources(m_RenderPass);

		m_Resources = new RenderResources();
		m_Resources->Initialize(m_RenderPass);

		m_RenderContextManager = &Application::GetModule<RenderContextManager>();
	}

	void RenderEngine::ClearDrawLists()
	{
		HZR_PROFILE_FUNCTION();
		//Clear cameras
		for (auto& renderer : m_DrawList)
		{
			renderer.WorldRenderer->m_CameraData.clear();
			renderer.WorldRenderer->m_RendererExtraCalls.clear();
			renderer.Pipelines.clear();
		}
		m_DrawList.clear();
		m_CurrentDrawContext = 0;
	}

	void RenderEngine::PreRender()
	{
		HZR_PROFILE_FUNCTION();
		auto& drawList = GetDrawList();
		m_LineRenderer.BeginScene();
		m_QuadRenderer.BeginScene();

		drawList.WorldRenderer->Render();
		drawList.WorldRenderer->OnRenderExtra();

		m_LineRenderer.EndScene();
		m_QuadRenderer.EndScene();
	}

	void RenderEngine::ShadowPass(Ref<RenderCommandBuffer> commandBuffer)
	{
		HZR_PROFILE_FUNCTION();
		auto& drawList = GetDrawList();
	}
	void RenderEngine::PreDepthPass(Ref<RenderCommandBuffer> commandBuffer)
	{
		HZR_PROFILE_FUNCTION();
		auto& drawList = GetDrawList();
	}
	void RenderEngine::GeometryPass(Ref<RenderCommandBuffer> commandBuffer)
	{
		HZR_PROFILE_FUNCTION();
		auto& drawList = GetDrawList();

		for (auto& [pipeline, meshList] : drawList.MeshList)
		{
			if (!pipeline->IsValid()) continue;
			commandBuffer->BindPipeline(pipeline);

			for (auto& mesh : meshList)
			{
				m_Resources->ModelUniformBuffer->SetData(glm::value_ptr(mesh.Transform), sizeof(glm::mat4));
				commandBuffer->BindVertexBuffer(mesh.VertexBuffer);
				commandBuffer->Draw(mesh.Count, mesh.IndexBuffer);
			}
		}
	}
	void RenderEngine::CompositePass(Ref<RenderCommandBuffer> commandBuffer)
	{
		HZR_PROFILE_FUNCTION();
		auto& drawList = GetDrawList();

		for (auto& [pipeline, usages] : drawList.Pipelines)
		{
			commandBuffer->BindPipeline(pipeline);
			for (auto& data : usages)
			{
				commandBuffer->Draw(data.Count);
			}
		}
	}
	void RenderEngine::LightPass(Ref<RenderCommandBuffer> commandBuffer)
	{
		HZR_PROFILE_FUNCTION();
		auto& drawList = GetDrawList();

		LightingData data;
		data.DirectionLightCount = drawList.DirectionalLights.size();

		for (uint32_t i = 0; i < data.DirectionLightCount; i++)
		{
			auto& light = drawList.DirectionalLights[i];
			data.Lights[i].Direction = glm::vec4(glm::normalize(light.Direction), 1.0);
			data.Lights[i].Color = glm::vec4(light.Color, light.Intensity);
		}

		if (drawList.Environment.size() > 0)
		{
			for (auto& [map, environmentData] : drawList.Environment)
			{
				data.SkyLightIntensity = environmentData.IBLContribution;
				break;
			}
		}

		//Update buffers
		commandBuffer->BindUniformBuffer(m_Resources->LightUniformBuffer, m_Resources->LightUniformBuffer->GetBinding());
		m_Resources->LightUniformBuffer->SetData(&data, sizeof(LightingData));
	}
	void RenderEngine::DrawEnvironmentMap(Ref<RenderCommandBuffer> commandBuffer)
	{

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

		Ref<RenderCommandBuffer> commandBuffer = m_RenderContextManager->GetWindow().GetSwapchain()->GetSwapchainBuffer();

		for (auto& worldDrawList : m_DrawList)
		{
			//Not camera dependant
			PreRender();
			LightPass(commandBuffer);
			ShadowPass(commandBuffer);
			PreDepthPass(commandBuffer);

			for (auto& camera : worldDrawList.WorldRenderer->m_CameraData)
			{
				glm::mat4 inverseProjection = glm::inverse(camera.Projection);
				glm::mat4 inverseView = glm::inverse(camera.View);

				CameraData data = {};
				data.ViewProjection = camera.Projection * inverseView;
				data.Projection = camera.Projection;
				data.View = inverseView;
				data.InverseViewProjection = camera.View * inverseProjection;
				data.Position = glm::vec4(camera.Position, 1.0);
				data.ZNear = camera.ZNear;
				data.ZFar = camera.ZFar;

				m_Resources->CameraUniformBuffer->SetData(&data, sizeof(CameraData));
				commandBuffer->BindUniformBuffer(m_Resources->CameraUniformBuffer, 0);

				commandBuffer->BeginRenderPass(camera.RenderPass);
				CompositePass(commandBuffer);
				GeometryPass(commandBuffer);
				DrawEnvironmentMap(commandBuffer);
				commandBuffer->EndRenderPass();
			}

			m_CurrentDrawContext++;
			break;
		}
	}
}
