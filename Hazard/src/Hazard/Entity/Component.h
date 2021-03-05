#pragma once

#include "Hazard/Math/Math.h"
#include "Hazard/Math/Color.h"

#include "glm/glm.hpp"

#include "Hazard/Rendering/RenderUtils.h"

namespace Hazard::ECS {

enum Projection { Perspective = 0, Orthographic };
#pragma region General


	struct TagComponent
	{
		std::string tag;
	};
	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 GetTransformMat4()
		{
			

			return Math::ToTransformMatrix(Translation, Rotation, Scale);
		}
	};

#pragma endregion
#pragma region Lights 

	struct SkyLightComponent {
		float intensity = 1.0f;
		Color tint;
	};
	struct DirectionalLightComponent {
		float intensity = 1.0f;
		Color tint;
	};
	struct PointLightComponent {
		float intensity = 1.0f;
		Color tint;

		float radius = 10.0f;
	};

#pragma endregion
	
	struct CameraComponent {
		
		Projection type = Projection::Perspective;
		float fov = 10.0f;

		glm::mat4 projection;
		float width, height;

		int GetProjectionType() { return type; }
		void SetProjection(Projection t) { type = t; RecalculateProjection(width, height); }

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
		Rendering::Texture* texture = nullptr;
	};
	struct BatchComponent {
		Color tint;
		uint32_t size;
	};

	struct Camera {

		glm::mat4 viewProjection;
		glm::vec3 position;

		CameraComponent* component;
		Camera() = default;

		Camera(CameraComponent& cam, glm::mat4 viewProjection, glm::vec3 pos) : component(&cam), position(pos) {
			this->viewProjection = viewProjection;
		}
	};
}