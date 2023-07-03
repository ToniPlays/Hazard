#pragma once

#include <glm/glm.hpp>
#include "Batch.h"

#include "Backend/Core/GPUBuffer.h"
#include "Backend/Core/Pipeline.h"
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
		LineRenderer() = default;
		~LineRenderer() = default;
		LineRenderer(const LineRenderer&) = delete;
		LineRenderer(const LineRenderer&&) = delete;

		void Init();
		void BeginScene();
		void EndScene();
		void BeginBatch();
		void Flush();

		void SubmitLine(const glm::vec3& startPos, const glm::vec3& endPos, const glm::vec4& color);

		void SetRenderPass(Ref<HazardRenderer::RenderPass> renderPass);
		void CreateResources(Ref<HazardRenderer::RenderPass> renderPass);

	private:
		Batch<LineVertex>* m_LineBatch = nullptr;
		LineRendererData m_Data;

		AssetHandle m_PipelineHandle;
		Ref<HazardRenderer::GPUBuffer> m_VertexBuffer;
		Ref<HazardRenderer::RenderPass> m_RenderPass;
	};
}
