#pragma once

#include "Color.h"
#include "MathCore.h"
#include "Hazard/Assets/Asset.h"
#include "UID.h"
#include "Ref.h"
#include "Hazard/Rendering/Mesh/Mesh.h"
#include "Hazard/Rendering/Mesh/Material.h"
#include "Core/Instance/ScriptObject.h"
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
		glm::quat Rotation;
		glm::vec3 EulerRotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		glm::mat4 TransformMatrix = glm::mat4(1.0f);
		bool Dirty = true;

	public:

		const glm::vec3& GetTranslation() const { return Translation; }
		const glm::quat& GetRotation() const { return Rotation; }
		const glm::vec3& GetRotationEuler() const { return EulerRotation; }
		const glm::vec3& GetScale() const { return Scale; }

		void SetTranslation(const glm::vec3& translation) { Translation = translation; Dirty = true; }
		void SetRotation(const glm::quat& rotation) { Rotation = rotation; Dirty = true; }
		void SetRotation(const glm::vec3& rotation) 
		{ 
			EulerRotation = rotation;
			Rotation = glm::quat(EulerRotation);
			Dirty = true; 
		}
		void SetScale(const glm::vec3& scale) { Scale = scale; Dirty = true; }
		void SetTransform(const glm::mat4& transform)
		{
			Math::DecomposeTransform(transform, Translation, Rotation, Scale);
			TransformMatrix = transform;
		}

		void AddTranslation(const glm::vec3& translation) { Translation += translation; Dirty = true; }
		void AddScale(const glm::vec3& scale) { Scale += scale; Dirty = true; }

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
		AssetHandle m_MeshHandle;
        Ref<Material> m_MaterialHandle;
	};
}
