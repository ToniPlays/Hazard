#pragma once

#include "Hazard/Math/Color.h"
#include "Hazard/Math/Math.h"

namespace Hazard::ECS {

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
	struct ScriptComponent {
		std::string moduleName = "Test";
	};
}
