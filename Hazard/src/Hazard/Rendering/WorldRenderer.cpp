
#include "hzrpch.h"
#include "WorldRenderer.h"
#include "RenderCommand.h"

namespace Hazard::Rendering 
{
	WorldRenderer::WorldRenderer() 
	{
		s_Renderers.push_back(this);
		CreateResources();
	}
	WorldRenderer::WorldRenderer(WorldRendererSettings* settings) : m_Settings(std::move(*settings))
	{
		s_Renderers.push_back(this);
		CreateResources();
	}
	WorldRenderer::~WorldRenderer() 
	{
	}
	void WorldRenderer::SetViewport(uint32_t width, uint32_t height) 
	{
		m_Settings.ViewportSize = { width, height };
		m_Resize = true;
	}
	bool WorldRenderer::Prepare()
	{
		return false;
	}
	void WorldRenderer::Begin(Ref<RenderCommandBuffer> cmdBuffer, CommandQueue* queue)
	{
		RenderContextCommand::BeginRenderPass(cmdBuffer, m_RenderPass);
		RenderContextCommand::SetLineWidth(cmdBuffer, m_Settings.LineWidth);
	}
	void WorldRenderer::End(Ref<RenderCommandBuffer> cmdBuffer)
	{
		RenderContextCommand::EndRenderPass(cmdBuffer);
		m_FrameBuffer->Unbind();
	}
	void WorldRenderer::Invalidate()
	{
		if (!m_Resize) return;

		m_Resize = false;
		//m_FrameBuffer->Resize(m_Settings.ViewportSize.x, m_Settings.ViewportSize.y);
	}
	void WorldRenderer::CreateResources()
	{
		FrameBufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.SwapChainTarget = m_Settings.SwapchainTarget;
		frameBufferInfo.AttachmentCount = 2;
		frameBufferInfo.Attachments = { { ImageFormat::RGBA }, { ImageFormat::Depth } };
		frameBufferInfo.ClearOnLoad = true;
		frameBufferInfo.ClearColor = m_Settings.ClearColor;
		frameBufferInfo.DebugName = "WorldRenderer";
		frameBufferInfo.Width = m_Settings.ViewportSize.x;
		frameBufferInfo.Height = m_Settings.ViewportSize.y;

		m_FrameBuffer = FrameBuffer::Create(&frameBufferInfo);

		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = "WorldRenderer";
		renderPassInfo.pTargetFrameBuffer = m_FrameBuffer;

		m_RenderPass = RenderPass::Create(&renderPassInfo);
		m_Resize = true;
	}
}
