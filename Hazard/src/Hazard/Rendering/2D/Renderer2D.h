#pragma once

#include "../RenderEngine.h"
#include "../Image/Image2D.h"
#include "../Batch.h"
#include "../WorldRenderer.h"

#include "Hazard/RenderContext/Pipeline/Pipeline.h"
#include "Hazard/Rendering/Queue/RenderCommandBuffer.h"

namespace Hazard::Rendering 
{
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	constexpr uint8_t quadVertexCount = 4;

	struct Vertex2D 
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoords;
		float TextureIndex;
	};

	struct Quad 
	{
		glm::mat4 Transform;
		glm::vec4 Color;
		Ref<Texture2D> Texture;
	};

	struct Renderer2DData 
	{
		size_t MaxQuadCount;
		size_t MaxVertices;
		size_t MaxIndices;
		uint32_t Samplers;
		std::vector<Ref<Texture2D>> TextureSlots;
		float TextureIndex = 0.0f;
		glm::vec4 QuadVertexPos[4];
	};

	class Renderer2D 
	{
	public:
		Renderer2D(RenderEngineCreateInfo* info, Ref<RenderCommandBuffer> buffer);
		~Renderer2D();

		void Submit(Quad quad);

		void SetTargetRenderPass(Ref<RenderPass> renderPass);
		void BeginWorld(WorldRenderFlags_ flags);
		void BeginBatch();
		void Flush();
		void EndWorld();
		void Recreate(Ref<RenderPass> renderPass);
		float FindTexture(const Ref<Texture2D>& texture);

	private:
		Renderer2DData m_Data;
		Ref<Pipeline> m_Pipeline;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		Batch<Vertex2D> m_QuadBatch;
		Ref<RenderCommandBuffer> m_RenderCommandBuffer;
		WorldRenderFlags_ m_CurrentFlags;
	};
}