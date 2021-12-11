#pragma once

#include "../RenderEngine.h"
#include "../Pipeline/Pipeline.h"
#include "../Image/Image2D.h"
#include "../RenderCommandBuffer.h"
#include "../Batch.h"


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
		Color Color;
		Ref<Texture2D> Texture;
	};

	struct Renderer2DData 
	{
		size_t MaxQuadCount;
		size_t MaxVertices;
		size_t MaxIndices;
		uint32_t Samplers;
		std::vector<Ref<Texture2D>> TextureSlots;
		uint32_t TextureIndex = 0;
		glm::vec4 QuadVertexPos[4];
	};

	class Renderer2D 
	{
	public:
		Renderer2D(RenderEngineCreateInfo* info, Ref<RenderCommandBuffer> buffer);
		~Renderer2D();

		void Submit(Quad quad);

		void SetTargetRenderPass(Ref<RenderPass> renderPass);

		void BeginWorld(const RenderPassData& renderPassData);
		void BeginBatch();
		void Flush();
		void EndWorld();
		void Recreate(Ref<RenderPass> renderPass);
		float FindTexture(const Ref<Texture2D>& texture);

	private:
		Renderer2DData m_Data;
		Ref<Pipeline> m_Pipeline;

		Batch<Vertex2D> m_QuadBatch;
		Ref<RenderCommandBuffer> m_RenderCommandBuffer;
	};
}