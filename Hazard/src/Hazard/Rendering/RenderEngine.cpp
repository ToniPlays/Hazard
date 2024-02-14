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

		FrameBufferCreateInfo frameBufferInfo = {
			.DebugName = "RenderEngine",
			.Width = 1920,
			.Height = 1080,
			.AttachmentCount = 3,
			.Attachments = { { ImageFormat::RGBA, { ImageFormat::RED32I, FramebufferBlendMode::None }, ImageFormat::Depth } },
			.SwapChainTarget = false,
			.ClearOnLoad = true,
		};

		m_FrameBuffer = FrameBuffer::Create(&frameBufferInfo);

		RenderPassCreateInfo renderPassInfo = {
			.DebugName = "RenderEngine",
			.pTargetFrameBuffer = m_FrameBuffer,
		};

		m_RenderPass = RenderPass::Create(&renderPassInfo);

		s_Resources = hnew RenderResources();
		m_RenderContextManager = &Application::Get().GetModule<RenderContextManager>();

		m_RenderGraph = CreateRasterGraph(this);

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
			//Render from every camera

			uint32_t cameraIndex = 0;
			for (auto& camera : worldDrawList.WorldRenderer->GetCameraData())
			{
				GraphBeginData data = {
					.Camera = {
						.ViewProjection = camera.Projection * camera.View,
						.Projection = camera.Projection,
						.View = camera.View,
						.Position = glm::vec4(camera.Position, 1.0),
					},
					.CameraDescriptor = worldDrawList.WorldRenderer->GetCameraDescriptor(cameraIndex),
					.GraphicsBuffer = commandBuffer,
					.OutputRenderpass = camera.RenderPass,
				};

				std::vector<MeshData> meshes;
				meshes.reserve(worldDrawList.MeshInstances.size());

				for (auto& [key, mesh] : worldDrawList.MeshInstances)
					meshes.push_back(mesh);

				m_RenderGraph->SetResource("GeometryPass", meshes.data(), meshes.size() * sizeof(MeshData));
				m_RenderGraph->SetResource("SkyboxPass", &worldDrawList.Environment, sizeof(EnvironmentData));

				m_RenderGraph->Execute(&data);
				cameraIndex++;

			}
			m_CurrentDrawContext++;
		}
		m_RenderGraph->Reset();
		ClearDrawLists();
	}
}
