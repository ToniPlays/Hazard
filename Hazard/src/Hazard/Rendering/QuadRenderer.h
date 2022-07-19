#pragma once

#include <glm/glm.hpp>
#include "Batch.h"

#include "Backend/Core/Pipeline/Buffers.h"
#include "Backend/Core/Pipeline/Pipeline.h"

namespace Hazard 
{
	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		float TextureIndex;
	};

	struct QuadRendererData
	{
		size_t MaxQuadCount;
		size_t MaxVertices;
		size_t MaxIndices;
		uint32_t Samplers;
		//std::vector<Ref<Texture2D>> TextureSlots;
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

		void BeginScene();
		void EndScene();

		void BeginBatch();

		void Flush();
		void SubmitQuad(const glm::mat4& transform, glm::vec4 color);
		void SubmitQuad(const glm::mat4& transform, glm::vec4 color, float textureID);

		void SetRenderPass(Ref<HazardRenderer::RenderPass> renderPass) {
			if (renderPass == m_RenderPass) return;
			m_RenderPass = renderPass;
			CreateResources();
		};
		void CreateResources();

	private:

	private:
		Batch<QuadVertex> m_QuadBatch;
		QuadRendererData m_Data;

		Ref<HazardRenderer::Pipeline> m_Pipeline;
		Ref<HazardRenderer::VertexBuffer> m_VertexBuffer;
		Ref<HazardRenderer::IndexBuffer> m_IndexBuffer;
		Ref<HazardRenderer::RenderPass> m_RenderPass;
	};
}