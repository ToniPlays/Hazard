#pragma once

#include <hzrpch.h>
#include "glm/glm.hpp"

namespace Hazard::Rendering {

	struct Vertex3D {
		glm::vec3 position	= { 0, 0, 0 };
		glm::vec4 color		= { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec3 normals	= { 0, 0, 0 };
		glm::vec2 texCoords = { 0, 0 };
	};
}