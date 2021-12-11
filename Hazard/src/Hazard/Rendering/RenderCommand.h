#pragma once

#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Pipeline/FrameBuffer.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "RenderCommandBuffer.h"
#include "RenderEngine.h"
#include "Hazard/Entity/Component.h"

namespace Hazard::Rendering 
{
	class RenderCommand 
	{
		friend class RenderEngine;
	public:
		static RenderAPI GetAPI() { return s_Api; }

		static void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) { RenderContextCommand::BeginRenderPass(buffer, renderPass); }
		static void EndRenderPass(Ref<RenderCommandBuffer> buffer) { RenderContextCommand::EndRenderPass(buffer); }

		static void DrawQuad(const glm::mat4& transform, const Color& tint);
		static void DrawQuad(const glm::mat4& transform, const Color& tint, const Ref<Texture2D>& texture);
		static void DrawQuad(const ECS::SpriteRendererComponent& comp, const ECS::TransformComponent& tc);
		static void DrawQuadTextured(const ECS::SpriteRendererComponent& comp, const ECS::TransformComponent& tc);
		static void DrawRotatedQuad() {};
		static void DrawScreenSpaceQuad() {};

		//Debug rendering
		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const Color& color);

		static void DrawMesh() {};
		static void DrawAnimatedMesh() {};

		static RenderStats GetStats() { return s_Engine->GetStats(); };
		static Ref<Texture2D> GetWhiteTexture() { return s_Engine->m_WhiteTexture; }

	private:
		
		static void Clear();

		inline static RenderEngine* s_Engine;
		inline static RenderAPI s_Api;
	};
}