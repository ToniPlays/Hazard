#pragma once

#include <glm/glm.hpp>
#include "Batch.h"

#include "Core/Rendering/GPUBuffer.h"
#include "Core/Rendering/Pipeline.h"
#include "Hazard/Rendering/Vertices.h"
#include "Hazard/Rendering/Mesh/Material.h"

namespace Hazard
{

	struct LineRendererData
	{
		uint64_t MaxLineCount;
		uint64_t MaxVertices;
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

		void CreateResources();

	private:
		Batch<LineVertex>* m_LineBatch = nullptr;
		LineRendererData m_Data;

		Ref<Material> m_Material;
		Ref<HazardRenderer::GPUBuffer> m_VertexBuffer;
	};
}
