#pragma once

#include <hzrpch.h>
#include "glm/glm.hpp"
#include "Hazard/RenderContext/RenderUtils.h"

namespace Hazard::Rendering {

	struct Quad {
		glm::mat4 transform;
		glm::vec4 color;
		Texture* texture;

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
}