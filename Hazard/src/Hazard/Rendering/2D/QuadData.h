#pragma once

#include <hzrpch.h>
#include "glm/glm.hpp"

namespace Hazard::Rendering {

	struct Quad {
		glm::mat4 transform;
		glm::vec4 color;

		Quad(glm::mat4 transform, glm::vec4 color) : transform(transform), color(color) {};
	};

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
	};
}