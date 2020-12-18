#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Hazard/Rendering/Textures/Texture2D.h"

namespace Hazard {
	struct TransformComponent {
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;

		glm::mat4 GetTransform() const {
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position);
			glm::quat rot = glm::quat(glm::vec3(glm::radians(Rotation.z), glm::radians(Rotation.y), glm::radians(Rotation.x)));

			return transform * glm::mat4(rot) * glm::scale(glm::mat4(1.0), Scale);
		}
	};
	struct CameraComponent
	{
		enum CameraType { Perspective, Orthographic };
		CameraType type;
		float fovSize = 20;

		CameraComponent() = default;

	};
	struct TagComponent
	{
		std::string Tag = "";
		TagComponent() = default;
		TagComponent(std::string tag) : Tag(tag) {}
	};
	struct ScriptComponent
	{
		std::string file = "";

		ScriptComponent() = default;
	};
	struct MeshComponent
	{

		std::string mesh = "";
		MeshComponent() = default;
	};
	struct MeshRendererComponent
	{
		float material;
		MeshRendererComponent() = default;
	};
	struct SpriteRenderer {
		Texture2D* texture = nullptr;
		SpriteRenderer() = default;
	};
}