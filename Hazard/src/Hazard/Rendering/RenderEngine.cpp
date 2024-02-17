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

		s_Resources = hnew RenderResources();
		m_RenderContextManager = &Application::Get().GetModule<RenderContextManager>();

		Ref<RenderCommandBuffer> cmdBuffer = m_RenderContextManager->GetWindow().GetSwapchain()->GetSwapchainBuffer();

		m_GeometryRenderer.Init(cmdBuffer);
		m_EnvironmentRenderer.Init(cmdBuffer);
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

		s_Resources->Initialize();
		m_QuadRenderer.Init();
		m_QuadRenderer.CreateResources();

		m_LineRenderer.Init();
		m_LineRenderer.CreateResources();

		m_CircleRenderer.Init();
		m_CircleRenderer.CreateResources();
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

		auto& window = m_RenderContextManager->GetWindow();
		Ref<RenderCommandBuffer> commandBuffer = window.GetSwapchain()->GetSwapchainBuffer();

		uint32_t cameraDataOffset = 0;

		for (auto& worldDrawList : m_DrawList)
		{
			CollectGeometry();

			//Update common rendergraph resources
			//Render from every camera

			uint32_t cameraIndex = 0;
			for (auto& camera : worldDrawList.WorldRenderer->GetCameraData())
			{
				CameraData cam = {
					.ViewProjection = camera.Projection * camera.View,
					.Projection = camera.Projection,
					.View = camera.View,
					.Position = glm::vec4(camera.Position, 1.0),
				};

				BufferCopyRegion camRegion = {
					.Size = sizeof(CameraData),
					.Offset = cameraDataOffset,
					.Data = &cam,
				};

				s_Resources->CameraUniformBuffer->SetData(camRegion);

				Ref<DescriptorSet> cameraDescriptor = worldDrawList.WorldRenderer->GetCameraDescriptor(cameraIndex);
				cameraDescriptor->Write(0, s_Resources->CameraUniformBuffer, sizeof(CameraData), camRegion.Offset);

				if (worldDrawList.Environment.RadianceMap)
				{
					cameraDescriptor->Write(1, 0, worldDrawList.Environment.RadianceMap, s_Resources->DefaultImageSampler, false);
					cameraDescriptor->Write(2, 0, worldDrawList.Environment.IrradianceMap, s_Resources->DefaultImageSampler, false);
					cameraDescriptor->Write(3, 0, s_Resources->BRDFLut->GetSourceImage(), s_Resources->DefaultImageSampler, false);
				}
				else
				{
					cameraDescriptor->Write(1, 0, s_Resources->WhiteCubemap, s_Resources->DefaultImageSampler, false);
					cameraDescriptor->Write(2, 0, s_Resources->WhiteCubemap, s_Resources->DefaultImageSampler, false);
					cameraDescriptor->Write(3, 0, m_RenderContextManager->GetWindow().GetContext()->GetDefaultResources().WhiteTexture, s_Resources->DefaultImageSampler, false);
				}


				m_GeometryRenderer.Prepare(worldDrawList.GeometryPass, camera.RenderPass, cameraDescriptor);
				m_EnvironmentRenderer.Prepare(worldDrawList.Environment, camera.RenderPass);

				commandBuffer->BeginRenderPass(camera.RenderPass);
				m_GeometryRenderer.Render();
				m_EnvironmentRenderer.Render(cameraDescriptor);
				commandBuffer->EndRenderPass();

				cameraIndex++;
				cameraDataOffset += camRegion.Size;
			}

			m_CurrentDrawContext++;
		}
	}
}
