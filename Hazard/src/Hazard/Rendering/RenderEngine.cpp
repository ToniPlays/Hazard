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
		s_Resources->Initialize(m_RenderPass);

		m_QuadRenderer.Init();
		m_QuadRenderer.CreateResources(m_RenderPass);
		m_LineRenderer.Init();
		m_LineRenderer.CreateResources(m_RenderPass);
		m_CircleRenderer.Init();
		m_CircleRenderer.CreateResources(m_RenderPass);
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
		m_CircleRenderer.BeginScene();

		drawList.WorldRenderer->Render();
		drawList.WorldRenderer->OnRenderExtra();

		m_QuadRenderer.EndScene();
		m_LineRenderer.EndScene();
		m_CircleRenderer.EndScene();
	}
	void RenderEngine::CreateRasterizedRenderGraph()
	{
		std::vector<RenderGraphStage> stages;
		stages.reserve(2);

		InputResource meshInstructions = {};
		meshInstructions.Name = "MeshInputInstructions";
		meshInstructions.UsageFlags = INPUT_RESOURCE_INSTRUCTIONS;

		InputResource meshResources = {};
		meshResources.Name = "MeshInputBuffers";
		meshResources.UsageFlags = INPUT_RESOURCE_RESOURCE_LIST;

		InputResource pipelineResources = {};
		pipelineResources.Name = "Pipelines";
		pipelineResources.UsageFlags = INPUT_RESOURCE_RESOURCE_LIST;

		InputResource meshPushConstantBuffers = {};
		meshPushConstantBuffers.Name = "MeshStageInputPushConstants";
		meshPushConstantBuffers.UsageFlags = INPUT_RESOURCE_RESOURCE_LIST;

		InputResource skyboxInstructions = {};
		skyboxInstructions.Name = "SkyboxInstructions";
		skyboxInstructions.UsageFlags = INPUT_RESOURCE_INSTRUCTIONS;

		std::vector<InputResource> skyboxInputResources = { skyboxInstructions, meshResources, pipelineResources };
		std::vector<InputResource> meshInputResources = { meshInstructions, meshResources, pipelineResources, meshPushConstantBuffers };

		StageDescriptor globalMeshDescriptor = {};
		globalMeshDescriptor.DebugName = "MeshStageSet 0";
		globalMeshDescriptor.Set = 0;
		globalMeshDescriptor.DescriptorSet = s_Resources->WorldDescriptor;


		auto& skyboxPass = stages.emplace_back();
		skyboxPass.DependencyCount = 0;
		skyboxPass.pDependencies = nullptr;
		skyboxPass.InputCount = skyboxInputResources.size();
		skyboxPass.pInputs = skyboxInputResources.data();
		skyboxPass.DescriptorCount = 1;
		skyboxPass.pStageDescriptors = &globalMeshDescriptor;

		auto& geometryPass = stages.emplace_back();
		geometryPass.DependencyCount = 1;
		geometryPass.pDependencies = &skyboxPass;
		geometryPass.InputCount = meshInputResources.size();
		geometryPass.pInputs = meshInputResources.data();
		geometryPass.DescriptorCount = 1;
		geometryPass.pStageDescriptors = &globalMeshDescriptor;

		RenderGraphCreateInfo graphInfo = {};
		graphInfo.DebugName = "RasterizedRenderGraph";
		graphInfo.StageCount = 2;
		graphInfo.pStages = stages.data();

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

				//Update materials
				Ref<Material> defaultMaterial = AssetManager::GetAsset<Material>(s_Resources->PBRMaterialHandle);
				Ref<Pipeline> defaultPipeline = AssetManager::GetAsset<AssetPointer>(defaultMaterial->GetPipeline())->Value.As<Pipeline>();
				Ref<Material> skyboxMaterial = AssetManager::GetAsset<Material>(s_Resources->SkyboxMaterialHandle);
				Ref<Pipeline> skyboxPipeline = AssetManager::GetAsset<AssetPointer>(skyboxMaterial->GetPipeline())->Value.As<Pipeline>();

				defaultPipeline->SetRenderPass(camera.RenderPass);
				skyboxPipeline->SetRenderPass(camera.RenderPass);
				commandBuffer->BeginRenderPass(camera.RenderPass);

				m_RasterizedRenderGraph->SetInput("MeshInputInstructions", worldDrawList.GeometryPassInstructions.data(), worldDrawList.GeometryPassInstructions.size() * sizeof(GraphInstruction));
				m_RasterizedRenderGraph->SetInput("MeshInputBuffers", worldDrawList.Buffers.Data(), worldDrawList.Buffers.Size() * sizeof(ResourceReference));
				m_RasterizedRenderGraph->SetInput("Pipelines", worldDrawList.Pipelines.Data(), worldDrawList.Pipelines.Size() * sizeof(ResourceReference));
				m_RasterizedRenderGraph->SetInput("MeshStageInputPushConstants", worldDrawList.PushConstantBuffers.data(), worldDrawList.PushConstantBuffers.size() * sizeof(ResourceReference));

				m_RasterizedRenderGraph->SetInput("SkyboxInstructions", worldDrawList.SkyboxInstructions.data(), worldDrawList.SkyboxInstructions.size() * sizeof(GraphInstruction));

				m_RasterizedRenderGraph->Execute(commandBuffer);
				commandBuffer->EndRenderPass();
			}
			m_CurrentDrawContext++;
		}
	}
}
