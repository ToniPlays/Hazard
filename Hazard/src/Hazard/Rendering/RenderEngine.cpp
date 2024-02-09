#include "hzrpch.h"
#include "RenderEngine.h"
#include "HRenderer.h"
#include "Mesh/MeshAssetLoader.h"
#include "Mesh/MaterialAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/Texture2D.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Math/Time.h"

#include "RenderGraphs/RasterRenderGraph.h"

#include <glm/glm.hpp>
#include "Environment/EnvironmentAssetLoader.h"

namespace Hazard
{
	RenderEngine::RenderEngine(RendererCreateInfo* createInfo) : Module("RenderEngine")
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;
		HRenderer::s_Engine = this;

		AssetManager::RegisterLoader<MeshAssetLoader>(AssetType::Mesh);
		AssetManager::RegisterLoader<MaterialAssetLoader>(AssetType::Material);
		AssetManager::RegisterLoader<EnvironmentAssetLoader>(AssetType::EnvironmentMap);

		FrameBufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.DebugName = "RenderEngine";
		frameBufferInfo.AttachmentCount = 3;
		frameBufferInfo.Attachments = { { ImageFormat::RGBA, { ImageFormat::RED32I, FramebufferBlendMode::None }, ImageFormat::Depth } };
		frameBufferInfo.ClearOnLoad = true;
		frameBufferInfo.Width = 1920;
		frameBufferInfo.Height = 1080;
		frameBufferInfo.SwapChainTarget = false;

		m_FrameBuffer = FrameBuffer::Create(&frameBufferInfo);

		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = "RenderEngine";
		renderPassInfo.pTargetFrameBuffer = m_FrameBuffer;

		m_RenderPass = RenderPass::Create(&renderPassInfo);

		s_Resources = hnew RenderResources();
		m_RenderContextManager = &Application::GetModule<RenderContextManager>();

		m_RenderGraph = CreateRasterGraph();
	}

	void RenderEngine::ClearDrawLists()
	{
		HZR_PROFILE_FUNCTION();
		//Clear cameras
		for (auto& renderer : m_DrawList)
		{
			renderer.WorldRenderer->m_CameraData.clear();
			renderer.WorldRenderer->m_RendererExtraCalls.Clear();
		}

		m_DrawList.clear();
		m_CurrentDrawContext = 0;
	}

	void RenderEngine::CollectGeometry()
	{
		HZR_PROFILE_FUNCTION();
		auto& drawList = GetDrawList();

		m_QuadRenderer.BeginScene();
		m_LineRenderer.BeginScene();
		m_CircleRenderer.BeginScene();

		drawList.WorldRenderer->Render();
		drawList.WorldRenderer->OnRenderExtra();

		m_QuadRenderer.EndScene();
		m_LineRenderer.EndScene();
		m_CircleRenderer.EndScene();
	}
	void RenderEngine::Init()
	{
		ShaderLibrary::Init(m_RenderContextManager->GetWindow().GetWindowInfo().SelectedAPI);

		s_Resources->Initialize(m_RenderPass);

		m_QuadRenderer.Init();
		m_QuadRenderer.CreateResources(m_RenderPass);
		m_LineRenderer.Init();
		m_LineRenderer.CreateResources(m_RenderPass);
		m_CircleRenderer.Init();
		m_CircleRenderer.CreateResources(m_RenderPass);
	}
	void RenderEngine::Update()
	{
		HZR_PROFILE_FUNCTION();
		
	}
	void RenderEngine::Render()
	{
		HZR_PROFILE_FUNCTION();
		HZR_TIMED_FUNCTION();

		Ref<RenderCommandBuffer> commandBuffer = m_RenderContextManager->GetWindow().GetSwapchain()->GetSwapchainBuffer();

		for (auto& worldDrawList : m_DrawList)
		{
			CollectGeometry();

			//Update common rendergraph resources
			// 
			//Render from every camera

			for (auto& camera : worldDrawList.WorldRenderer->GetCameraData())
			{
				CameraData data = {};
				data.Projection = camera.Projection;
				data.View = camera.View;
				data.ViewProjection = camera.Projection * camera.View;
				data.Position = glm::vec4(camera.Position, 1.0);

				BufferCopyRegion region = {};
				region.Data = &data;
				region.Size = sizeof(CameraData);
				region.Offset = 0;

				s_Resources->CameraUniformBuffer->SetData(region);
				if (worldDrawList.Environment.Pipeline)
					worldDrawList.Environment.Pipeline->SetRenderPass(camera.RenderPass);

				m_RenderGraph->SetStageActive("SkyboxPass", worldDrawList.Environment.Pipeline);
				m_RenderGraph->SetResource("GeometryPass", worldDrawList.GeometryMeshes.data(), worldDrawList.GeometryMeshes.size() * sizeof(GeometryMesh));
				m_RenderGraph->SetResource("SkyboxPass", &worldDrawList.Environment, sizeof(EnvironmentData));

				commandBuffer->BeginRenderPass(camera.RenderPass);
				m_RenderGraph->Execute(commandBuffer);
				commandBuffer->EndRenderPass();
			}
			m_CurrentDrawContext++;
		}
		ClearDrawLists();
	}
}
