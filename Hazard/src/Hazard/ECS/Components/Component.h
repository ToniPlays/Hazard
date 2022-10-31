#pragma once

#include "Color.h"
#include "MathCore.h"
#include "Hazard/Assets/Asset.h"
#include "UID.h"
#include "Ref.h"
#include "Hazard/Rendering/Mesh/Mesh.h"
#include "HazardScript.h"
#include "ComponentBase.h"

namespace Hazard {

	struct TagComponent : public ComponentBase
	{
		std::string Tag;
		UID Uid;
		bool Visible = true;
		bool ReceiveUpdate = true;
	};

	struct TransformComponent : public ComponentBase
	{
	private:
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		glm::mat4 TransformMatrix;
		bool Dirty = true;

	public:

		const glm::vec3& GetTranslation() const { return Translation; }
		const glm::vec3& GetRotation() const { return Rotation; }
		const glm::vec3& GetScale() const { return Scale; }

		void SetTranslation(const glm::vec3& translation) { Translation = translation; Dirty = true; }
		void SetRotation(const glm::vec3& rotation) { Rotation = rotation; Dirty = true; }
		void SetScale(const glm::vec3& scale) { Scale = scale; Dirty = true; }

		const glm::mat4& GetTransformMat4()
		{
			HZR_TIMED_FUNCTION();
			if (Dirty)
			{
				TransformMatrix = Math::ToTransformMatrix(Translation, Rotation, Scale);
				Dirty = false;
			}
			return TransformMatrix;
		}
		inline glm::mat4 GetTransformNoScale() const
		{
			HZR_TIMED_FUNCTION();
			return Math::ToTransformMatrix(Translation, Rotation);
		}
		inline glm::quat GetOrientation() const
		{
			return glm::quat(Rotation);
		}
	};

	struct ScriptComponent : public ComponentBase
	{
		std::string ModuleName;
		Ref<HazardScript::ScriptObject> m_Handle;
	};
	struct MeshComponent : public ComponentBase
	{
		Ref<Mesh> m_MeshHandle;
		float Metalness = 0.0f;
		float Roughness = 1.0f;
		bool CastShadows = false;
	};
}
