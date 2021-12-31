#pragma once

#include <hzrpch.h>
#include "glm/glm.hpp"

namespace Hazard::Rendering {

	struct Vertex3D 
	{
		glm::vec3 Position	= { 0, 0, 0 };
		glm::vec4 Color		= Color::White;
		glm::vec3 Normals	= { 0, 0, 0 };
		glm::vec2 TexCoords = { 0, 0 };
	};
}