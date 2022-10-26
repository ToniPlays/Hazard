#include "hzrpch.h"
#include "RenderEngine.h"
#include "HRenderer.h"
#include "Mesh/MeshAssetLoader.h"
#include "Environment/EnvironmentAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/Texture2D.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Math/Time.h"

namespace Hazard
{
	RenderEngine::RenderEngine(RendererCreateInfo* createInfo) : Module("RenderEngine")
	{
		HZR_PROFILE_FUNCTION();
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

		Ref<AssetPointer> whiteTextureCore = AssetPointer::Create(Image2D::Create(&info), AssetType::Image);

		m_WhiteTexture = Ref<Texture2DAsset>::Create(whiteTextureCore);

		FrameBufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.DebugName = "RenderEngine";
		frameBufferInfo.AttachmentCount = 4;
		frameBufferInfo.Attachments = { { ImageFormat::RGBA16F, ImageFormat::Depth } };
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

		m_Resources = hnew RenderResources();
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
				ModelData data = {};
				data.Transform = mesh.Transform;
				data.Metalness = mesh.Metalness;
				data.Roughness = mesh.Roughness;
				data.Flags = 0;

				m_Resources->ModelUniformBuffer->SetData(&data, sizeof(ModelData));
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

		LightingData data = {};
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
				data.IBLContribution = environmentData.IBLContribution;
				data.SkyLightIntensity = environmentData.Intensity;

				auto& radiance = environmentData.Map->RadianceMap;
				auto& irradiance = environmentData.Map->IrradianceMap;
				auto& prefilter = environmentData.Map->PreFilterMap;
				auto& lut = environmentData.Map->BRDFLut;

				auto& shader = m_Resources->PbrPipeline->GetShader();
				shader->Set("u_RadianceMap", 0, radiance->Value.As<CubemapTexture>());
				shader->Set("u_IrradianceMap", 0, irradiance->Value.As<CubemapTexture>());
				shader->Set("u_PrefilterMap", 0, prefilter->Value.As<CubemapTexture>());
				shader->Set("u_BRDFLut", 0, environmentData.Map->BRDFLut->Value.As<Image2D>());
				break;
			}
		}
		else
		{
			data.IBLContribution = 0.0f;
			data.SkyLightIntensity = 1.0f;

			auto& shader = m_Resources->PbrPipeline->GetShader();
			shader->Set("u_RadianceMap", 0, m_Resources->WhiteCubemap);
			shader->Set("u_IrradianceMap", 0, m_Resources->WhiteCubemap);
			shader->Set("u_PrefilterMap", 0, m_Resources->WhiteCubemap);
			shader->Set("u_BRDFLut", 0, m_WhiteTexture->GetSourceImageAsset()->Value.As<Image2D>());
		}
		UtilityUniformData utilData = {};
		utilData.Time = Time::s_Time;

		m_Resources->UtilityUniformBuffer->SetData(&utilData, sizeof(UtilityUniformData));
		commandBuffer->BindUniformBuffer(m_Resources->UtilityUniformBuffer);

		//Update buffers
		commandBuffer->BindUniformBuffer(m_Resources->LightUniformBuffer);
		m_Resources->LightUniformBuffer->SetData(&data, sizeof(LightingData));
	}
	void RenderEngine::DrawEnvironmentMap(Ref<RenderCommandBuffer> commandBuffer)
	{
		HZR_PROFILE_FUNCTION();
		auto& drawList = GetDrawList();

		if (drawList.Environment.size() == 0) return;
		for (auto& [map, environmentData] : drawList.Environment)
		{
			auto& radiance = environmentData.Map->RadianceMap;

			m_Resources->SkyboxPipeline->GetShader()->Set("u_CubeMap", 0, radiance->Value.As<CubemapTexture>());
			commandBuffer->BindPipeline(m_Resources->SkyboxPipeline);
			commandBuffer->Draw(6);
			return;
		}
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
				data.ViewProjection = camera.Projection * camera.View;
				data.Projection = camera.Projection;
				data.View = camera.View;
				data.InverseViewProjection = inverseView * inverseProjection;
				data.Position = inverseView[3];
				data.ZNear = camera.ZNear;
				data.ZFar = camera.ZFar;

				m_Resources->CameraUniformBuffer->SetData(&data, sizeof(CameraData));
				commandBuffer->BindUniformBuffer(m_Resources->CameraUniformBuffer);

				commandBuffer->BeginRenderPass(camera.RenderPass);
				GeometryPass(commandBuffer);
				DrawEnvironmentMap(commandBuffer);
				CompositePass(commandBuffer);
				commandBuffer->EndRenderPass();
			}

			m_CurrentDrawContext++;
			break;
		}
	}
}
