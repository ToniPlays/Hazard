#include "hzrpch.h"
#include "RenderEngine.h"
#include "HRenderer.h"
#include "Mesh/MeshAssetLoader.h"
#include "Mesh/MaterialAssetLoader.h"
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
		AssetManager::RegisterLoader<MaterialAssetLoader>(AssetType::Material);

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
		s_Resources->Initialize(m_RenderPass);

		m_QuadRenderer.Init();
		m_QuadRenderer.CreateResources(m_RenderPass);
		m_LineRenderer.Init();
		m_LineRenderer.CreateResources(m_RenderPass);
		m_RenderContextManager = &Application::GetModule<RenderContextManager>();

		CreateRasterizedRenderGraph();
	}

	void RenderEngine::ClearDrawLists()
	{
		HZR_PROFILE_FUNCTION();
		//Clear cameras
		for (auto& renderer : m_DrawList)
		{
			renderer.WorldRenderer->m_CameraData.clear();
			renderer.WorldRenderer->m_RendererExtraCalls.clear();
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

		drawList.WorldRenderer->Render();
		drawList.WorldRenderer->OnRenderExtra();

		m_QuadRenderer.EndScene();
		m_LineRenderer.EndScene();
	}
	void RenderEngine::CreateRasterizedRenderGraph()
	{
		std::vector<RenderGraphStage> stages;
		stages.reserve(2);

		InputResource meshInstructions = {};
		meshInstructions.Name = "MeshInputInstructions";
		meshInstructions.UsageFlags = INPUT_RESOURCE_INSTRUCTIONS;

		InputResource meshResources = {};
		meshResources.Name = "MeshStageInputResources";
		meshResources.UsageFlags = INPUT_RESOURCE_RESOURCE_LIST;

		std::vector<InputResource> inputResources = { meshInstructions, meshResources };

		BufferResource cameraBuffer = {};
		cameraBuffer.Set = 0;
		cameraBuffer.Binding = 0;
		cameraBuffer.Buffer = s_Resources->CameraUniformBuffer;

		auto& geometryPass = stages.emplace_back();
		geometryPass.DependencyCount = 0;
		geometryPass.pDependencies = nullptr;
		geometryPass.InputCount = inputResources.size();
		geometryPass.pInputs = inputResources.data();
		geometryPass.BufferCount = 1;
		geometryPass.pBuffers = &cameraBuffer;
		geometryPass.InputImageCount = 0;
		geometryPass.pInputImages = nullptr;
		geometryPass.OutputImageCount = 0;
		geometryPass.pOutputImages = nullptr;

		auto& compositePass = stages.emplace_back();
		compositePass.DependencyCount = 1;
		compositePass.pDependencies = &geometryPass;
		compositePass.InputImageCount = 0;
		compositePass.pInputImages = nullptr;
		compositePass.OutputImageCount = 0;
		compositePass.pOutputImages = nullptr;

		RenderGraphCreateInfo graphInfo = {};
		graphInfo.DebugName = "RasterizedRenderGraph";
		graphInfo.StageCount = 1;
		graphInfo.pStages = &geometryPass;

		m_RasterizedRenderGraph = RenderGraph::Create(&graphInfo);
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

		Ref<RenderCommandBuffer> commandBuffer = m_RenderContextManager->GetWindow().GetSwapchain()->GetSwapchainBuffer();

		for (auto& worldDrawList : m_DrawList)
		{
			CollectGeometry();

			for (auto& camera : worldDrawList.WorldRenderer->GetCameraData())
			{
				CameraData data = {};
				data.Projection = camera.Projection;
				data.View = camera.View;
				data.ViewProjection = camera.Projection * camera.View;

				BufferCopyRegion region = {};
				region.Data = &data;
				region.Size = sizeof(CameraData);
				region.Offset = 0;

				s_Resources->CameraUniformBuffer->SetData(region);

				commandBuffer->BeginRenderPass(camera.RenderPass);
				m_RasterizedRenderGraph->SetInput("MeshInputInstructions", worldDrawList.GeometryPassInstructions.data(), worldDrawList.GeometryPassInstructions.size() * sizeof(GraphInstruction));
				m_RasterizedRenderGraph->SetInput("MeshStageInputResources", worldDrawList.Buffers.data(), worldDrawList.Buffers.size() * sizeof(GraphInstruction));
				m_RasterizedRenderGraph->Execute(commandBuffer);

				commandBuffer->EndRenderPass();
			}
			m_CurrentDrawContext++;
		}
	}
}
