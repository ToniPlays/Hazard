#pragma once

#include <hzrpch.h>
#include "glm/glm.hpp"

namespace Hazard::Rendering {

	struct Quad {
		glm::vec3 position;
		glm::vec4 color;

		Quad(glm::vec3 pos, glm::vec4 color) : position(pos), color(color) {};
	};

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
	};
}