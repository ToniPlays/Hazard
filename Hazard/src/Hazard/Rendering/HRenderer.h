#pragma once

#include "Hazard/ECS/Component.h"
#include "HazardRenderer.h"
#include "RenderEngine.h"
#include "WorldRenderer.h"

namespace Hazard
{
	using namespace HazardRenderer;
	class HRenderer
	{
		friend class RenderEngine;
	public:
		static void SubmitWorldRenderer(WorldRenderer* renderer);
		static void SubmitSprite(const TransformComponent& transform, const SpriteRendererComponent& spriteRenderer);
		static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color);
		static void SubmitMesh(Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, Ref<Pipeline>& pipeline);
		static void SubmitMesh(Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, Ref<Pipeline>& pipeline, uint32_t count);
	private:
		inline static RenderEngine* s_Engine;
	};
}