#pragma once

#include <glm/glm.hpp>
#include "Backend/Core/Pipeline/BufferLayout.h"

namespace Hazard
{
	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoords;
		float TextureIndex;

		static HazardRenderer::BufferLayout Layout() {
			using namespace HazardRenderer;
			return {
				{ "a_Position",		ShaderDataType::Float3 },
				{ "a_Color",		ShaderDataType::Float4 },
				{ "a_TextureCoords",ShaderDataType::Float2 },
				{ "a_TextureIndex",	ShaderDataType::Float  }
			};
		}
	};
	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};
}