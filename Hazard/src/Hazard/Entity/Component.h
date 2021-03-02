#pragma once

#include "Hazard/Math/Math.h"
#include "Hazard/Math/Color.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Hazard::ECS {

	enum Projection { Perspective, Orthographic };

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
		
		Projection type = Projection::Perspective;
		float fov = 10.0f;

		glm::mat4 projection;
		float width, height;

		void RecalculateProjection(float w, float h) {

			width = w;
			height = h;

			float aspectRatio = width / height;

			if (type == Perspective) {
				projection = glm::perspective(glm::radians(fov), aspectRatio, 0.01f, 1000.0f);
			} 
			else
			{
				float orthoLeft = -fov * aspectRatio * 0.5f;
				float orthoRight = fov * aspectRatio * 0.5f;
				float orthoBottom = -fov * 0.5f;
				float orthoTop = fov * 0.5f;

				projection = glm::ortho(orthoLeft, orthoRight,
					orthoBottom, orthoTop, -1000.0f, 1000.0f);
			}
		}
	};
	struct SpriteRendererComponent {
		Color tint;
	};

	struct Camera {

		glm::mat4 viewProjection;

		CameraComponent* component;
		Camera() = default;

		Camera(CameraComponent& cam, glm::mat4 viewProjection) : component(&cam) {
			this->viewProjection = viewProjection;
		}
	};
}