#pragma once

#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Pipeline/FrameBuffer.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "Queue/RenderCommandBuffer.h"
#include "RenderEngine.h"
#include "Mesh/Mesh.h"

namespace Hazard::Rendering 
{
	class RenderCommand 
	{
		friend class RenderEngine;
	public:
		static RenderAPI GetAPI() { return s_Api; }

		static void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) { RenderContextCommand::BeginRenderPass(buffer, renderPass); }
		static void EndRenderPass(Ref<RenderCommandBuffer> buffer) { RenderContextCommand::EndRenderPass(buffer); }
		static void SetLineWidth(float lineWidth);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& tint, int id = 0);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& tint, const Ref<Texture2D>& texture, int id = 0);

		//TODO: Remove these?
		static void DrawScreenSpaceQuad() {};
		static void DrawCircle(const glm::mat4& transform, float radius = 1.0f, float thickness = 0.02f, const glm::vec4& tint = Color::White, int id = 0);

		static void DrawCustomGeometry(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline, int id = 0);
		static void DispatchPipeline(Ref<Pipeline> pipeline, uint32_t count, int id = 0);
		static void DispatchPipelinePostPass(Ref<Pipeline> pipeline, uint32_t count, int id = 0);

		//Debug rendering
		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int id = 0);
		static void DrawRectangle(const glm::mat4& transform, const glm::vec4& color, int id = 0);

		static void DrawMesh(Ref<Mesh> mesh, const glm::mat4& transform, int id = 0);
		static void DrawAnimatedMesh() {};

		static Ref<Texture2D> GetWhiteTexture() { return s_Engine->m_WhiteTexture; }

	private:
		static void Clear();

		inline static RenderEngine* s_Engine;
		inline static RenderAPI s_Api;
	};
}
