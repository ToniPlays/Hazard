
#include <hzrpch.h>
#include <glm/glm.hpp>

namespace Hazard::Rendering {
	struct Camera
	{
		glm::mat4 projection;
		glm::mat4 view;
		glm::vec4 clearColor;

		Camera() = default;
		Camera(glm::mat4 projection, glm::mat4 view, glm::vec4 color) : clearColor(color), 
			projection(projection), view(view) {
		}
	};
}