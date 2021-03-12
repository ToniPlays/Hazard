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
		glm::vec4 clearColor;

		glm::vec3 position;

		Camera() = default;
		Camera(glm::mat4 projection, glm::mat4 view, glm::vec3 position, glm::vec4 color) : clearColor(color), 
			projection(projection), position(position), view(view) {
		}
	};
}
#endif // !CAMERA