
#include <hzrpch.h>
#include "RenderContextCommand.h"

namespace Hazard::Rendering {

	RenderContext* RenderContextCommand::m_Context;

	void RenderContextCommand::SetTitle(const char* title)
	{
		HZR_GUARD(m_Context);
		m_Context->GetWindow().SetWindowTitle(title);
	}
	float RenderContextCommand::GetWindowWidth()
	{
		return (float)m_Context->GetWindow().GetWidth();
	}
	float RenderContextCommand::GetWindowHeight()
	{
		return (float)m_Context->GetWindow().GetHeight();
	}
	bool RenderContextCommand::IsFullscreen()
	{
		return m_Context->GetWindow().IsFullscreen();
	}
	void RenderContextCommand::SetFullscreen(bool fullscreen)
	{
		m_Context->GetWindow().SetFullscreen(fullscreen);
	}
	bool RenderContextCommand::IsVsync()
	{
		return m_Context->GetWindow().IsVSync();
	}
	void RenderContextCommand::SetVsync(bool vsync)
	{
		m_Context->GetWindow().SetVSync(vsync);
	}
	void RenderContextCommand::BeginRenderPass(Ref<RenderCommandBuffer> cmdBuffer, Ref<RenderPass> renderPass)
	{
		Submit([cmdBuffer, renderPass]() mutable {
			m_Context->GetContext().BeginRenderPass(cmdBuffer, renderPass);
			});
	}
	void RenderContextCommand::EndRenderPass(Ref<RenderCommandBuffer> cmdBuffer)
	{
		Submit([cmdBuffer]() mutable {
			m_Context->GetContext().EndRenderPass(cmdBuffer);
			});
	}
	void RenderContextCommand::SetLineWidth(Ref<RenderCommandBuffer> cmdBuffer, float lineWidth)
	{
		Submit([cmdBuffer, lineWidth]() mutable {
			m_Context->GetContext().SetLineWidth(cmdBuffer, lineWidth);
			});
	}
	void RenderContextCommand::DrawGeometry(Ref<RenderCommandBuffer> cmdBuffer, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline)
	{
		DrawGeometry(cmdBuffer, vertexBuffer, indexBuffer, pipeline, indexBuffer->GetCount());
	}
	void RenderContextCommand::DrawGeometry(Ref<RenderCommandBuffer> cmdBuffer, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline, uint32_t count)
	{
		Submit([cmdBuffer, vertexBuffer, indexBuffer, pipeline, count]() mutable {
			vertexBuffer->Bind(cmdBuffer);
			indexBuffer->Bind(cmdBuffer);
			pipeline->Bind(cmdBuffer);
			pipeline->Draw(cmdBuffer, count);
			});
	}
	void RenderContextCommand::DrawGeometry_RT(Ref<RenderCommandBuffer> cmdBuffer, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline)
	{
		DrawGeometry_RT(cmdBuffer, vertexBuffer, indexBuffer, pipeline, indexBuffer->GetCount());
	}
	void RenderContextCommand::DrawGeometry_RT(Ref<RenderCommandBuffer> cmdBuffer, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline, uint32_t count)
	{
		vertexBuffer->Bind(cmdBuffer);
		indexBuffer->Bind(cmdBuffer);
		pipeline->Bind(cmdBuffer);
		pipeline->Draw(cmdBuffer, count);
	}
	void RenderContextCommand::DrawGeometryArray(Ref<RenderCommandBuffer> cmdBuffer, Ref<VertexBuffer> vertexBuffer, Ref<Pipeline> pipeline, uint32_t count)
	{
		Submit([cmdBuffer, vertexBuffer, pipeline, count]() mutable {
			vertexBuffer->Bind(cmdBuffer);
			pipeline->Bind(cmdBuffer);
			pipeline->DrawArrays(cmdBuffer, count);
			});
	}
	void RenderContextCommand::DispatchPipeline(Ref<RenderCommandBuffer> cmdBuffer, Ref<Pipeline> pipeline, uint32_t count)
	{
		Ref<Pipeline> instance = pipeline;
		Submit([cmdBuffer, instance, count]() mutable {
			instance->Bind(cmdBuffer);
			instance->DrawArrays(cmdBuffer, count);
			});
	}
	void RenderContextCommand::SetDebugCallback(ErrorCallback& callback)
	{
		m_Context->GetContext().SetErrorListener(callback);
	}
	void RenderContextCommand::SetClearColor(const Color& color)
	{
		m_Context->GetContext().SetClearColor({ color.r, color.g, color.b, color.a });
	}
}
