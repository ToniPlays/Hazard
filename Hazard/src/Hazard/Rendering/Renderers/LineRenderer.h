#pragma once

#include <glm/glm.hpp>
#include "Batch.h"

#include "Backend/Core/Pipeline/Buffers.h"
#include "Backend/Core/Pipeline/Pipeline.h"
#include "Hazard/Rendering/Vertices.h"

namespace Hazard
{

	struct LineRendererData
	{
		size_t MaxLineCount;
		size_t MaxVertices;
	};


	class LineRenderer
	{
	public:
		LineRenderer();
		~LineRenderer() = default;
		LineRenderer(const LineRenderer&) = delete;
		LineRenderer(const LineRenderer&&) = delete;

		void Init();

		void BeginScene();
		void EndScene();

		void BeginBatch();

		void Flush();
		void SubmitLine(const glm::vec3& startPos, const glm::vec3& endPos, const glm::vec4& color);

		void SetRenderPass(Ref<HazardRenderer::RenderPass> renderPass) {
			if (renderPass == m_RenderPass) return;
			m_RenderPass = renderPass;
			CreateResources(renderPass);
		};
		void CreateResources(Ref<HazardRenderer::RenderPass> renderPass);

	private:
		Batch<LineVertex>* m_LineBatch = nullptr;
		LineRendererData m_Data;

		Ref<AssetPointer> m_Pipeline;
		Ref<HazardRenderer::VertexBuffer> m_VertexBuffer;
		Ref<HazardRenderer::RenderPass> m_RenderPass;
	};
}