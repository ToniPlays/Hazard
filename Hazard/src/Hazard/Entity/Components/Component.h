#pragma once

#include "Hazard/Math/Color.h"
#include "Hazard/Math/Math.h"
#include "Hazard/Rendering/Mesh/Mesh.h"


/*
General entity components
*/
namespace Hazard::ECS {

	//Entity tag component and identity, non removable
	struct TagComponent
	{
		std::string tag;
		bool visible = true;
	};
	//Entity transformation component, non removable
	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 GetTransformMat4()
		{
			return Math::ToTransformMatrix(Translation, Rotation, Scale);
		}
		glm::mat4 GetTransformNoScale()
		{
			return Math::ToTransformMatrix(Translation, Rotation);
		}
	};
	//Script component for entities
	struct ScriptComponent {
		std::string moduleName = "";
	};
	struct MeshComponent {
		Rendering::Mesh* mesh;
	};
}
