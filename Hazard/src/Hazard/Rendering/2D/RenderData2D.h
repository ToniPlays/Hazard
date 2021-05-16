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
		Ref<Shader> QuadShader;

		uint32_t QuadIndexCount = 0;
		uint32_t TextureIndex = 1;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture>, 8> TextureSlots;
		glm::vec4 QuadVertexPos[4];
	};
}