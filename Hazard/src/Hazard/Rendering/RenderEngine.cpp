#pragma once

#include <hzrpch.h>
#include "RenderEngine.h"
#include "RenderCommand.h"
#include "Loaders/Loaders.h"
#include "Hazard/Assets/AssetManager.h"
#include "2D/Renderer2D.h"
#include "Mesh/MeshFactory.h"
#include "WorldRenderer.h"

namespace Hazard::Rendering
{
	RenderEngine::RenderEngine(RenderEngineCreateInfo* info) : Module("RenderEngine")
	{
		HZR_PROFILE_FUNCTION();
		SetActive(true);

		RenderCommand::s_Engine = this;
		RenderCommand::s_Api = Application::GetModule<RenderContext>().GetCurrentAPI();

		AssetManager::RegisterLoader<TextureLoader>(AssetType::Image);
		AssetManager::RegisterLoader<MeshLoader>(AssetType::Mesh);
		m_RenderCommandBuffer = RenderCommandBuffer::Create("RenderEngine");
		m_Queue = new RenderCommandQueue();

		WindowResizeEvent e = { 1920, 1080 };
		OnResize(e);

		uint32_t data = 0xFFFFFFFF;
		Texture2DCreateInfo whiteTextureInfo = {};
		whiteTextureInfo.Width = 1;
		whiteTextureInfo.Height = 1;
		whiteTextureInfo.Data = &data;
		whiteTextureInfo.Usage = ImageUsage::Texture;
		whiteTextureInfo.Format = ImageFormat::RGBA;

		m_WhiteTexture = Texture2D::Create(&whiteTextureInfo);
		m_Renderer2D = new Renderer2D(info, m_RenderCommandBuffer);

	}
	RenderEngine::~RenderEngine()
	{
		delete m_Renderer2D;
	}
	void RenderEngine::Render()
	{
		for (auto& worldRenderer : WorldRenderer::s_Renderers)
		{
			worldRenderer->Invalidate();
		}
		m_RenderCommandBuffer->Begin();

		for (auto& worldRenderer : WorldRenderer::s_Renderers)
		{
			worldRenderer->Begin(m_RenderCommandBuffer, m_Queue);
			if (worldRenderer->IsValid()) {
				if ((uint32_t)worldRenderer->m_Settings.Flags & (uint32_t)WorldRenderFlags::Enabled) {
					m_RenderPassData.ViewProjection = worldRenderer->m_Settings.Camera->GetViewPprojection();
					m_Renderer2D->SetTargetRenderPass(worldRenderer->GetRenderPass());
					m_Renderer2D->BeginWorld(m_RenderPassData);
					m_Queue->Excecute();
					m_Renderer2D->EndWorld();
				}
				worldRenderer->End(m_RenderCommandBuffer);
			}
		}
		m_RenderCommandBuffer->End();
		m_RenderCommandBuffer->Submit();
		m_Queue->Clear();
	}
	bool RenderEngine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(RenderEngine::OnResize));
	}
	void RenderEngine::Close()
	{
		HZR_PROFILE_FUNCTION();
	}
	bool RenderEngine::OnResize(WindowResizeEvent& e)
	{
		return false;
	}
}
