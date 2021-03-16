#pragma once

#ifndef CAMERA_STRUCT
#define CAMERA_STRUCT

#include <hzrpch.h>
#include <glm/glm.hpp>
#include "Sky/BackgroundRenderer.h"


namespace Hazard::Rendering {

	struct Camera
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::vec4 clearColor = {0, 0, 0, 1.0};
		glm::vec3 position;

		BackgroundRenderer* renderer;

		Camera() = default;
		Camera(glm::mat4 projection, glm::mat4 view, glm::vec3 position, BackgroundRenderer* rd) : projection(projection), 
			position(position), view(view), renderer(rd) {}
	};
}
#endif // !CAMERA