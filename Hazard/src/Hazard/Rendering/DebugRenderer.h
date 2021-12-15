#pragma once

#include "Hazard/Core/Core.h"
#include <glm/glm.hpp>
#include "2D/Renderer2D.h"

namespace Hazard::Rendering 
{
	struct DebugRendererData 
	{
		uint32_t MaxLineCount;
		uint32_t MaxVertices;
		float LineWidth = 1.0f;
	};
	struct LineVertex 
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};
	struct CircleVertex2D
	{
		glm::vec3 Position;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;
	};
	struct Circle
	{
		glm::mat4 Transform;
		glm::vec4 Color;
		float Radius;
		float Thickness;
		float Fade;
	};
	struct Line
	{
		glm::vec3 Start;
		glm::vec3 End;
		glm::vec4 Color = { 0.0f, 0.0f, 0.0f, 1.0f};
		Line() = default;
		Line(const glm::vec3& start, const glm::vec3& end) : Start(start), End(end) {}
	};

	class DebugRenderer 
	{
	public:
		DebugRenderer(uint32_t lines, Ref<RenderCommandBuffer> cmdBuffer);
		~DebugRenderer();

		void SetTargetRenderPass(Ref<RenderPass> renderPass);
		void BeginWorld(const RenderPassData& passData, WorldRenderFlags_ flags);
		void EndWorld();

		void BeginLineBatch();
		void FlushLines();
		void SubmitLine(Line line);

		void BeginCircleBatch();
		void FlushCircles();
		void SubmitCircle(Circle cirlce);

		void Recreate(Ref<RenderPass> renderPass);

	private:
		DebugRendererData m_RenderData;
		Ref<RenderCommandBuffer> m_CommandBuffer;
		WorldRenderFlags_ m_CurrentFlags;

		Ref<Pipeline> m_LinePipeline = nullptr;
		Ref<VertexBuffer> m_LineVertexBuffer;
		Batch<LineVertex> m_LineBatch;

		Ref<Pipeline> m_CirclePipeline = nullptr;
		Ref<VertexBuffer> m_CircleVertexBuffer;
		Ref<IndexBuffer> m_CircleIndexBuffer;
		Batch<CircleVertex2D> m_CircleBatch;

		glm::vec4 m_QuadPos[4];
	};
}