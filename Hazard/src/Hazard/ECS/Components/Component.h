#pragma once

#include "Color.h"
#include "MathCore.h"
#include "UID.h"

namespace HazardScript {
	class ScriptObject;
}

namespace Hazard {

	struct TagComponent
	{
		std::string Tag;
		UID Uid;
		bool Visible = true;
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };


		bool Dirty = true;

		inline glm::mat4 GetTransformMat4() const
		{
			return Math::ToTransformMatrix(Translation, Rotation, Scale);
		}
		inline glm::mat4 GetTransformNoScale() const
		{
			return Math::ToTransformMatrix(Translation, Rotation);
		}
		inline glm::quat GetOrientation() const
		{
			return glm::quat(Rotation);
		}
	};

	struct ScriptComponent 
	{
		std::string ModuleName;
		HazardScript::ScriptObject* m_Handle;
	};
	struct MeshComponent 
	{
		float ye;
	};
}
