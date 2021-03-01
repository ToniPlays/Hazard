#pragma once

#include "Hazard/Math/Math.h"
#include "Hazard/Math/Color.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Hazard::ECS {

	struct TagComponent
	{
		std::string tag;
	};
	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation =	{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale =		{ 1.0f, 1.0f, 1.0f };

		glm::mat4 GetTransformMat4()
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			
			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};
	struct CameraComponent {
		float fov = 1.0f;

		glm::mat4 projection;

		void RecalculateProjection(float w, float h) {
			float aspect = w / h * fov;
			projection = glm::ortho(-aspect, aspect, -fov, fov, -1000.0f, 1000.0f);

		}
	};
	struct SpriteRendererComponent {
		Color tint;
	};
}