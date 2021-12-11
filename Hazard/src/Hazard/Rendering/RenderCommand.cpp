#pragma once
#include <hzrpch.h>
#include "RenderCommand.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "2D/Renderer2D.h"
#include "DebugRenderer.h"

namespace Hazard::Rendering
{
	void RenderCommand::DrawQuad(const glm::mat4& transform, const Color& tint)
	{
		DrawQuad(transform, tint, s_Engine->m_WhiteTexture);
	}
	void RenderCommand::DrawQuad(const glm::mat4& transform, const Color& tint, const Ref<Texture2D>& texture)
	{
		Quad quad;
		quad.Transform = transform;
		quad.Color = tint;
		quad.Texture = texture;

		s_Engine->Submit([quad]() mutable 
			{
			s_Engine->Get2D().Submit(quad);
			});
	}
	void RenderCommand::DrawQuad(const ECS::SpriteRendererComponent& comp, const ECS::TransformComponent& tc)
	{
		glm::mat4 transform = Math::ToTransformMatrix(tc.Translation, tc.Rotation, tc.Scale);
		DrawQuad(transform, comp.Tint, comp.Texture);
	}
	void RenderCommand::DrawQuadTextured(const ECS::SpriteRendererComponent& comp, const ECS::TransformComponent& tc)
	{
		Quad quad;
		quad.Transform = Math::ToTransformMatrix(tc.Translation, tc.Rotation, tc.Scale);
		quad.Color = comp.Tint;
		quad.Texture = comp.Texture;

		s_Engine->Submit([quad]() mutable {
			s_Engine->Get2D().Submit(quad);
			});
	}
	void RenderCommand::DrawLine(const glm::vec3& start, const glm::vec3& end, const Color& color)
	{
		Line line;
		line.Start = start;
		line.End = end;
		line.Color = { color.r, color.g, color.b, color.a };

		s_Engine->Submit([line]() mutable {
			s_Engine->GetDebugRenderer().SubmitLine(line);
			});
	}
	void RenderCommand::Clear()
	{

	}
}
