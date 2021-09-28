#pragma once

#include "../RenderEngine.h"
#include "../Pipeline.h"


namespace Hazard::Rendering 
{

	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	constexpr uint8_t quadVertexCount = 4;

	struct Vertex2D 
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct Quad 
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct Renderer2DData 
	{
		uint32_t MaxQuadCount;
		uint32_t MaxVertices;
		uint32_t MaxIndices;
		uint32_t Samplers;
		uint32_t QuadIndexCount;

		uint32_t TextureIndex;
		Vertex2D* BufferBase;
		Vertex2D* BufferPtr;

		glm::vec4 QuadVertexPos[4];
	};

	class Renderer2D {
	public:
		Renderer2D(RenderEngineCreateInfo* info);
		~Renderer2D();

		void Render();
		void Submit(Quad quad);

		void BeginWorld();
		void BeginBatch();
		void Flush();

	private:
		Renderer2DData m_Data;
		Ref<Pipeline> m_Pipeline;
	};
}