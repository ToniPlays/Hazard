#pragma once

#include <glm/glm.hpp>
#include "Batch.h"

#include "Core/Rendering/GPUBuffer.h"
#include "Core/Rendering/Pipeline.h"
#include "Hazard/RenderContext/Texture2D.h"

#include "Hazard/Rendering/Mesh/Material.h"

#include "Hazard/Rendering/Vertices.h"

namespace Hazard 
{
	struct QuadRendererData
	{
		uint64_t MaxQuadCount;
		uint64_t MaxVertices;
		uint64_t MaxIndices;
		uint32_t Samplers;

		std::vector<Ref<HazardRenderer::Image2D>> TextureSlots;
		float TextureIndex = 0.0f;
		glm::vec4 QuadVertexPos[4];
	};

	class QuadRenderer 
	{
	public:
		QuadRenderer() = default;
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

		Ref<Material> m_Material;
		Ref<HazardRenderer::GPUBuffer> m_VertexBuffer;
		Ref<HazardRenderer::GPUBuffer> m_IndexBuffer;
		Ref<HazardRenderer::RenderPass> m_RenderPass;
	};
}
