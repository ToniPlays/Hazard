#pragma once

#include <glm/glm.hpp>

namespace HazardRenderer 
{
	class Camera
	{
	public:
		Camera() = default;

		virtual glm::mat4 GetProjection() const = 0;
		virtual glm::mat4 GetView() const = 0;
		virtual glm::vec3 GetPosition() const = 0;

		virtual const float GetNearClipping() const = 0;
		virtual const float GetFarClipping() const = 0;
	};
}
