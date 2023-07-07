#pragma once

#include <glm/glm.hpp>
#include "Batch.h"

#include "Backend/Core/GPUBuffer.h"
#include "Backend/Core/Pipeline.h"
#include "Hazard/RenderContext/Texture2D.h"

#include "Hazard/Rendering/Mesh/Material.h"

#include "Hazard/Rendering/Vertices.h"

namespace Hazard
{
	struct CircleRendererData
	{
		size_t MaxCircleCount;
		size_t MaxVertices;
		size_t MaxIndices;
		glm::vec4 CircleVertexPos[4];
	};

	class CircleRenderer
	{
	public:
		CircleRenderer() = default;
		~CircleRenderer() = default;
		CircleRenderer(const CircleRenderer&) = delete;
		CircleRenderer(const CircleRenderer&&) = delete;

		void Init();
		void BeginScene();
		void EndScene();
		void BeginBatch();
		void Flush();

		void SubmitCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade);
		void SubmitBillboard(const glm::mat4& transform, const glm::mat4& view, const glm::vec4& color, float thickness, float fade);
		bool IsVisible(const glm::mat4& transform);

		void SetRenderPass(Ref<HazardRenderer::RenderPass> renderPass)
		{
			if (renderPass == m_RenderPass) return;
			m_RenderPass = renderPass;
			CreateResources(renderPass);
		};
		void CreateResources(Ref<HazardRenderer::RenderPass> renderPass);

	private:

		Batch<CircleVertex>* m_CicleBatch = nullptr;
		CircleRendererData m_Data;

		Ref<Material> m_Material;
		Ref<HazardRenderer::GPUBuffer> m_VertexBuffer;
		Ref<HazardRenderer::GPUBuffer> m_IndexBuffer;
		Ref<HazardRenderer::RenderPass> m_RenderPass;
	};
}
