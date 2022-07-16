
#include <hzrpch.h>
#include "WorldRenderer.h"
#include "HRenderer.h"

using namespace HazardRenderer;

namespace Hazard 
{
	WorldRenderer::WorldRenderer(WorldRendererSpec* spec) : m_Spec(*spec)
	{
		FrameBufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.DebugName = spec->DebugName;
		frameBufferInfo.SwapChainTarget = false;
		frameBufferInfo.AttachmentCount = 2;
		frameBufferInfo.ClearColor = { 0.05f, 0.05f, 0.05f, 1.0f };
		frameBufferInfo.Attachments = { { ImageFormat::RGBA }, { ImageFormat::Depth } };
		frameBufferInfo.AttachmentCount = 2;
		frameBufferInfo.Width = spec->Width;
		frameBufferInfo.Height = spec->Height;

		m_Output = FrameBuffer::Create(&frameBufferInfo);
		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = spec->DebugName;
		renderPassInfo.pTargetFrameBuffer = m_Output;

		m_RenderPass = RenderPass::Create(&renderPassInfo);
	}
	void WorldRenderer::Render() 
	{
		HRenderer::SubmitWorldRenderer(this);
	}
}