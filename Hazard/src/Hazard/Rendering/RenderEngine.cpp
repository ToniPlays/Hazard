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

		s_Resources = hnew RenderResources();
		s_Resources->Initialize(m_RenderPass);

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
            
            if(m_Settings.Raytraced)
                m_RaytracedRenderer->Render();
            else
                m_RasterizedRenderer->Render(commandBuffer, worldDrawList);
            
            m_CurrentDrawContext++;
        }
	}
}
