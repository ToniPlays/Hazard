#include "hzrpch.h"
#include "RenderEngine.h"
#include "HRenderer.h"
#include "Mesh/MeshAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/Texture2D.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Math/Time.h"

#include <glm/glm.hpp>

namespace Hazard
{
	RenderEngine::RenderEngine(RendererCreateInfo* createInfo) : Module("RenderEngine")
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;
		HRenderer::s_Engine = this;
		AssetManager::RegisterLoader<MeshAssetLoader>(AssetType::Mesh);

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

		m_Resources = hnew RenderResources();
		m_Resources->Initialize(m_RenderPass);

		m_RasterizedRenderer = hnew RasterizedRenderer();
		m_RaytracedRenderer = hnew RaytracedRenderer();

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

		m_QuadRenderer.BeginScene();
		m_LineRenderer.BeginScene();

		drawList.WorldRenderer->Render();
		drawList.WorldRenderer->OnRenderExtra();

		m_QuadRenderer.EndScene();
		m_LineRenderer.EndScene();
	}
	void RenderEngine::Update()
	{
		HZR_PROFILE_FUNCTION();
		ClearDrawLists();
	}
	void RenderEngine::Render()
	{
		HZR_PROFILE_FUNCTION();
		HZR_TIMED_FUNCTION();

		m_CurrentRenderer = m_Settings.Raytraced ? static_cast<BaseRenderer*>(m_RaytracedRenderer) : static_cast<BaseRenderer*>(m_RasterizedRenderer);

		Ref<RenderCommandBuffer> commandBuffer = m_RenderContextManager->GetWindow().GetSwapchain()->GetSwapchainBuffer();
		m_CurrentRenderer->SetCommandBuffer(commandBuffer);

		for (auto& worldDrawList : m_DrawList)
		{
			//Not camera dependant
			PreRender();
			m_CurrentRenderer->Prepare(m_RenderPass, worldDrawList);

			for (auto& camera : worldDrawList.WorldRenderer->m_CameraData)
			{
				glm::mat4 inverseProjection = glm::inverse(camera.Projection);
				glm::mat4 inverseView = glm::inverse(camera.View);

				CameraData data = {};
				data.ViewProjection = camera.Projection * camera.View;
				data.Projection = camera.Projection;
				data.InverseProjection = inverseProjection;
				data.View = camera.View;
				data.InverseView = inverseView;
				data.InverseViewProjection = inverseView * inverseProjection;

				UtilityUniformData utils = {};
				utils.CameraPos = data.InverseView[3];
				utils.Flags = m_Flags;

				{
					BufferCopyRegion region = {};
					region.Data = &data;
					region.Size = sizeof(CameraData);
					m_Resources->CameraUniformBuffer->SetData(region);
				}
				{
					BufferCopyRegion region = {};
					region.Data = &utils;
					region.Size = sizeof(UtilityUniformData);
					m_Resources->UtilityUniformBuffer->SetData(region);
				}

				commandBuffer->BindUniformBuffer(m_Resources->UtilityUniformBuffer);
				commandBuffer->BindUniformBuffer(m_Resources->CameraUniformBuffer);
				commandBuffer->BeginRenderPass(camera.RenderPass);

				m_CurrentRenderer->GeometryPass(worldDrawList.MeshList);
				m_CurrentRenderer->EnvironmentPass(worldDrawList.Environment);
				m_CurrentRenderer->CompositePass(worldDrawList.Pipelines);
				commandBuffer->EndRenderPass();
			}

			m_CurrentDrawContext++;
			break;
		}
	}
}
