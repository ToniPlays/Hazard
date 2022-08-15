#pragma once

#include "Color.h"
#include "MathCore.h"
#include "Hazard/Assets/Asset.h"
#include "UID.h"
#include "Ref.h"
#include "Hazard/Rendering/Mesh/Mesh.h"
#include "HazardScript.h"

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
		Ref<HazardScript::ScriptObject> m_Handle;
	};
	struct MeshComponent 
	{
		Ref<Mesh> m_MeshHandle;
	};
}
