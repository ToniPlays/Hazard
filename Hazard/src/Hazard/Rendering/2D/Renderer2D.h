#pragma once

#include "QuadData.h"

#include "../Buffers/VertexArray.h"
#include "../Buffers/Buffer.h"
#include "../Shader/Shader.h"
#include "../RenderContext.h"

#include "../RenderUtils.h"

namespace Hazard::Rendering {

	struct Renderer2DData {
		uint32_t MaxQuads;
		uint32_t MaxVertices;
		uint32_t MaxIndices;

		VertexArray* QuadVertexArray;
		VertexBuffer* QuadVertexBuffer;
		Shader* QuadShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		glm::vec4 QuadVertexPos[4];
	};

	class Renderer2D {
	public:
		Renderer2D(RenderContext* context);
		~Renderer2D();

		void Init(uint32_t size);

		void SubmitQuad(Quad quad);
		void BeginScene(glm::mat4 viewProjection);
		void BeginBatch();

		void Flush();
		void Close();

		void CollectStats(RenderStats& stats);

	private:
		Renderer2DData data;
		RenderContext* context;

		glm::mat4 viewProjection;
		RenderStats stats;
	};
}