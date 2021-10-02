#pragma once

#include "../RenderEngine.h"
#include "../Pipeline.h"
#include "../Texture/Texture2D.h"


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
		float TextureIndex;
	};

	struct Renderer2DData 
	{
		size_t MaxQuadCount;
		size_t MaxVertices;
		size_t MaxIndices;
		size_t QuadIndexCount;
		uint32_t Samplers;

		uint32_t TextureIndex;
		Vertex2D* BufferBase;
		Vertex2D* BufferPtr;

		glm::vec4 QuadVertexPos[4];
	};

	class Renderer2D {
	public:
		Renderer2D(RenderEngineCreateInfo* info);
		~Renderer2D();

		void Render(const RenderPassData& renderPassData);
		void Submit(Quad quad);

		void BeginWorld();
		void BeginBatch();
		void Flush();

	private:
		Renderer2DData m_Data;
		Ref<Pipeline> m_Pipeline;
	};
}