#pragma once
#include <hzrpch.h>
#include "RenderCommand.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "2D/Renderer2D.h"
#include "DebugRenderer.h"

namespace Hazard::Rendering
{
	void RenderCommand::SetLineWidth(float lineWidth)
	{
		s_Engine->Submit([lineWidth]() mutable {
			s_Engine->SetLineWidth(lineWidth);
			});
	}
	void RenderCommand::DrawQuad(const glm::mat4& transform, const glm::vec4& tint)
	{
		DrawQuad(transform, tint, s_Engine->m_WhiteTexture);
	}
	void RenderCommand::DrawQuad(const glm::mat4& transform, const glm::vec4& tint, const Ref<Texture2D>& texture)
	{
		Quad quad;
		quad.Transform = transform;
		quad.Color = tint;
		quad.Texture = texture;

		s_Engine->Submit([quad]() mutable {
			s_Engine->Get2D().Submit(quad);
			});
	}
	void RenderCommand::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color)
	{
		Line line = { start, end };
		line.Color = color;

		s_Engine->Submit([line]() mutable {
			s_Engine->GetDebugRenderer().SubmitLine(line);
			});
	}
	void RenderCommand::DrawRectangle(const glm::mat4& transform, const glm::vec4& color)
	{
		glm::vec3 topLeft		= transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
		glm::vec3 topRight		= transform * glm::vec4( 0.5f, 0.5f, 0.0f, 1.0f);
		glm::vec3 bottomRight	= transform * glm::vec4( 0.5f,-0.5f, 0.0f, 1.0f);
		glm::vec3 bottomLeft	= transform * glm::vec4(-0.5f,-0.5f, 0.0f, 1.0f);

		Line line1 = { topLeft, topRight };
		line1.Color = color;

		Line line2 = { topRight, bottomRight };
		line2.Color = color;

		Line line3 = { bottomRight, bottomLeft };
		line3.Color = color;

		Line line4 = { bottomLeft, topLeft };
		line4.Color = color;

		s_Engine->Submit([line1, line2, line3, line4]() mutable {
			DebugRenderer& rd = s_Engine->GetDebugRenderer();
			rd.SubmitLine(line1);
			rd.SubmitLine(line2);
			rd.SubmitLine(line3);
			rd.SubmitLine(line4);
			});
	}
	void RenderCommand::DrawCircle(const glm::mat4& transform, float radius, float thickness, const glm::vec4& tint)
	{
		Circle circle;
		circle.Transform = transform;
		circle.Color = tint;
		circle.Radius = radius;
		circle.Thickness = thickness;
		circle.Fade = 0.01f;

		s_Engine->Submit([circle]() {
			s_Engine->GetDebugRenderer().SubmitCircle(circle);
			});
	}
	void RenderCommand::DrawCustomGeometry(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline) {
		s_Engine->Submit([vertexBuffer, indexBuffer, pipeline]() mutable {
			s_Engine->DrawGeometry(vertexBuffer, indexBuffer, pipeline);
		});
	}

	void RenderCommand::DispatchPipeline(Ref<Pipeline> pipeline, uint32_t count)
	{
		s_Engine->Submit([pipeline, count]() mutable {
			s_Engine->DispatchPipeline(pipeline, count);
		});
	}
	void RenderCommand::DispatchPipelinePostPass(Ref<Pipeline> pipeline, uint32_t count)
	{
		s_Engine->SubmitPostPass([pipeline, count]() mutable {
			s_Engine->DispatchPipeline(pipeline, count);
			});
	}

	void RenderCommand::Clear()
	{

	}
}
