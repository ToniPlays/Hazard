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

		m_WhiteTexture = Ref<Hazard::Image2DAsset>::Create(&info);

		m_QuadRenderer.Init();
		m_QuadRenderer.CreateResources();
		m_LineRenderer.Init();
		m_LineRenderer.CreateResources();

		m_Resources = new RenderResources();

		m_RenderContextManager = &Application::GetModule<RenderContextManager>();
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
			commandBuffer->BindPipeline(pipeline);
			for (auto& mesh : meshList)
			{
				commandBuffer->BindVertexBuffer(mesh.VertexBuffer);
				commandBuffer->BindIndexBuffer(mesh.VertexBuffer);
			}
		}
	}
	void RenderEngine::CompositePass(Ref<RenderCommandBuffer> commandBuffer)
	{
		HZR_PROFILE_FUNCTION();
		auto& drawList = GetDrawList();
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
			ShadowPass(commandBuffer);
			PreDepthPass(commandBuffer);

			for (auto& camera : worldDrawList.WorldRenderer->m_CameraData)
			{
				commandBuffer->BeginRenderPass(camera.RenderPass);
				GeometryPass(commandBuffer);
				CompositePass(commandBuffer);
				commandBuffer->EndRenderPass();
			}

			m_CurrentDrawContext++;
			break;
		}
	}
}
