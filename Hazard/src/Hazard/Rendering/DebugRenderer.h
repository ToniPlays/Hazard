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
	};
	struct LineVertex 
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};
	struct Line
	{
		glm::vec3 Start;
		glm::vec3 End;
		glm::vec4 Color;
	};

	class DebugRenderer 
	{
	public:
		DebugRenderer(uint32_t lines, Ref<RenderCommandBuffer> cmdBuffer);
		~DebugRenderer();

		void SetTargetRenderPass(Ref<RenderPass> renderPass);
		void BeginWorld(const RenderPassData& renderPassData, WorldRenderFlags_ flags);
		void BeginBatch();
		void EndWorld();
		void Flush();
		void SubmitLine(Line line);

		void Recreate(Ref<RenderPass> renderPass);

	private:
		DebugRendererData m_RenderData;
		Ref<RenderCommandBuffer> m_CommandBuffer;
		WorldRenderFlags_ m_CurrentFlags;

		Ref<Pipeline> m_LinePipeline = nullptr;
		Ref<VertexBuffer> m_LineVertexBuffer;
		Batch<LineVertex> m_LineBatch;
	};
}