#pragma once

#include <glm/glm.hpp>

namespace Hazard::Rendering {
	struct Camera
	{
		glm::mat4 viewProjection;
		glm::vec4 clearColor;

		Camera() = default;
		Camera(glm::mat4 viewProjection, glm::vec4 color) : clearColor(color) {
			this->viewProjection = viewProjection;
		}
	};
}