#pragma once

#include "Hazard/Core/Core.h"
#include <glm/glm.hpp>

namespace Hazard::Rendering {
	class Camera {
	public:
		Camera() = default;

		virtual const glm::mat4& GetProjection() const = 0;
		virtual const glm::mat4& GetView() const = 0;
		virtual glm::mat4 GetViewPprojection() const = 0;
		virtual void SetViewport(uint32_t width, uint32_t heigth) = 0;
	};
}