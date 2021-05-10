#pragma once

#ifndef CAMERA_STRUCT
#define CAMERA_STRUCT

#include <hzrpch.h>
#include <glm/glm.hpp>

namespace Hazard::Rendering {

	struct Camera
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::vec3 position;

		Camera() = default;

		Camera(glm::mat4 projection, glm::mat4 view, glm::vec3 position) : projection(projection), 
			position(position), view(view) {}
	};
}
#endif // !CAMERA