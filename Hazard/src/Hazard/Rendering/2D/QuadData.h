#pragma once

#include <hzrpch.h>
#include "glm/glm.hpp"
#include "../RenderUtils.h"

namespace Hazard::Rendering {

	struct Quad {
		glm::mat4 transform;
		glm::vec4 color;
		uint8_t texture;

		Quad(glm::mat4 transform, glm::vec4 color, int16_t text) : transform(transform), 
			color(color) {
			texture = text == -1 ? RenderUtils::GetWhiteTexture()->GetID() : text;
		};
	};

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec3 texture;
		glm::vec3 surfaceNormal;
	};
}