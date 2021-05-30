#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard::Rendering 
{
	struct Renderer2DData {

		uint32_t MaxQuads = 0;
		uint32_t MaxVertices = 0;
		uint32_t MaxIndices = 0;

		VertexArray* QuadVertexArray = nullptr;
		VertexBuffer* QuadVertexBuffer = nullptr;
		Ref<GraphicsPipeline> QuadPipeline;

		uint32_t QuadIndexCount = 0;
		uint32_t TextureIndex = 1;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::vector<Ref<Texture>> TextureSlots;
		glm::vec4 QuadVertexPos[4];
	};
}