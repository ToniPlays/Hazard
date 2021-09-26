#pragma once

#include <hzrpch.h>
#include "glm/glm.hpp"

namespace Hazard::Rendering {

	struct Quad {
		glm::mat4 Transform;
		glm::vec4 Color;

		Quad(glm::mat4 transform, glm::vec4 color) : Transform(transform),
			Color(color)
		{
		};
	};

	struct Vertex {
		glm::vec3 position = { 0, 0, 0 };
		glm::vec4 color = { 1, 1, 1, 1 };
		glm::vec3 normals = { 0, 0, 0 };
		glm::vec2 texCoords = { 0, 0 };
	};
}