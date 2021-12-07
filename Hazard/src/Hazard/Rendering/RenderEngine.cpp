#pragma once

#include <hzrpch.h>
#include "RenderEngine.h"
#include "RenderCommand.h"
#include "Loaders/Loaders.h"
#include "Hazard/Assets/AssetManager.h"
#include "2D/Renderer2D.h"
#include "Mesh/MeshFactory.h"

namespace Hazard::Rendering
{
	RenderEngine::RenderEngine(RenderEngineCreateInfo* info, RenderAPI api) : Module("RenderEngine")
	{
		HZR_PROFILE_FUNCTION();
		SetActive(true);

		RenderCommand::s_Engine = this;
		RenderCommand::s_Api = api;

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

		FrameBufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.SwapChainTarget = false;
		frameBufferInfo.AttachmentCount = 2;
		frameBufferInfo.Attachments = { { ImageFormat::RGBA }, { ImageFormat::Depth } };
		frameBufferInfo.ClearOnLoad = true;
		frameBufferInfo.ClearColor = Color::Black;
		frameBufferInfo.DebugName = "Mesh3D";
		frameBufferInfo.Width = 1920;
		frameBufferInfo.Height = 1080;

		m_FrameBuffer = FrameBuffer::Create(&frameBufferInfo);

		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = "MainRenderPass";
		renderPassInfo.pTargetFrameBuffer = m_FrameBuffer;

		m_RenderPass = RenderPass::Create(&renderPassInfo);
		m_Renderer2D = new Renderer2D(info, m_RenderCommandBuffer);
		m_Renderer2D->Recreate(m_RenderPass);
	}
	RenderEngine::~RenderEngine()
	{
		delete m_Renderer2D;
	}
	void RenderEngine::Render()
	{
		m_RenderCommandBuffer->Begin();
		m_RenderPassData.ViewProjection = m_RenderingCamera->GetViewPprojection();
		
		RenderCommand::BeginRenderPass(m_RenderCommandBuffer, m_RenderPass);
		m_Renderer2D->BeginWorld(m_RenderPassData);

		m_Queue->Excecute();
		m_Renderer2D->EndWorld();

		RenderCommand::EndRenderPass(m_RenderCommandBuffer);
		m_RenderCommandBuffer->End();
		m_RenderCommandBuffer->Submit();
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
		if (e.GetWidth() == 0 || e.GetHeight() == 0 && !m_RenderPass->GetSpecs().TargetFrameBuffer->GetSpecification().SwapChainTarget) return false;

		if(m_RenderingCamera != nullptr)
			m_RenderingCamera->SetViewport(e.GetWidth(), e.GetHeight());
		return true;
	}
}
