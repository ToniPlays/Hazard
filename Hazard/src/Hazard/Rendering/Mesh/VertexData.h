#pragma once

#include <hzrpch.h>
#include "glm/glm.hpp"
#include "Hazard/Rendering/RenderUtils.h"

namespace Hazard::Rendering {

	struct Quad {
		glm::mat4 transform;
		glm::vec4 color;
		Ref<Texture> texture;

		Quad(glm::mat4 transform, glm::vec4 color, Texture* text) : transform(transform), 
			color(color) {
			texture = text;
		};
	};

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texture;
		float textureIndex;
	};

	struct Vertex {
		glm::vec3 position  = { 0, 0, 0 };
		glm::vec4 color     = { 1, 1, 1, 1 };
		glm::vec3 normals   = { 0, 0, 0 };
		glm::vec2 texCoords = { 0, 0 };
	};
}