#pragma once

#include <glm/glm.hpp>
#include "Batch.h"

#include "Backend/Core/Pipeline/Buffers.h"
#include "Backend/Core/Pipeline/Pipeline.h"
#include "Hazard/RenderContext/Texture2D.h"

#include "Hazard/Rendering/Vertices.h"

namespace Hazard 
{
	struct QuadRendererData
	{
		size_t MaxQuadCount;
		size_t MaxVertices;
		size_t MaxIndices;
		uint32_t Samplers;
		std::vector<Ref<HazardRenderer::Image2D>> TextureSlots;
		float TextureIndex = 0.0f;
		glm::vec4 QuadVertexPos[4];
	};

	class QuadRenderer 
	{
	public:
		QuadRenderer();
		~QuadRenderer() = default;
		QuadRenderer(const QuadRenderer&) = delete;
		QuadRenderer(const QuadRenderer&&) = delete;

		void Init();

		void BeginScene();
		void EndScene();

		void BeginBatch();

		void Flush();
		void SubmitQuad(const glm::mat4& transform, const glm::vec4& color, Ref<Texture2DAsset> texture);
		void SubmitBillboard(const glm::mat4& transform, const glm::mat4& view, const glm::vec4& color, Ref<Texture2DAsset> texture);
		bool IsVisible(const glm::mat4& transform);

		void SetRenderPass(Ref<HazardRenderer::RenderPass> renderPass) 
		{
			if (renderPass == m_RenderPass) return;
			m_RenderPass = renderPass;
			CreateResources(renderPass);
		};
		void CreateResources(Ref<HazardRenderer::RenderPass> renderPass);

	private:
		float GetImageIndex(const Ref<HazardRenderer::Image2D>& image);

	private:
		Batch<QuadVertex>* m_QuadBatch = nullptr;
		QuadRendererData m_Data;

		Ref<HazardRenderer::Pipeline> m_Pipeline;
		Ref<HazardRenderer::VertexBuffer> m_VertexBuffer;
		Ref<HazardRenderer::IndexBuffer> m_IndexBuffer;
		Ref<HazardRenderer::RenderPass> m_RenderPass;
	};
}