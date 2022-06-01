#pragma once

#include "Color.h"
#include "MathCore.h"
#include "UID.h"

namespace Hazard::ECS {

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

		inline glm::mat4 GetTransformMat4()
		{
			return Math::ToTransformMatrix(Translation, Rotation, Scale);
		}
		inline glm::mat4 GetTransformNoScale()
		{
			return Math::ToTransformMatrix(Translation, Rotation);
		}
		inline glm::quat GetOrientation() 
		{
			return glm::quat(Rotation);
		}
	};

	struct ScriptComponent 
	{
		std::string ModuleName;
	};
	struct VisualScriptComponent 
	{
		std::string Filename;
	};
	struct MeshComponent 
	{
		float ye;
		//Ref<Rendering::MeshAsset> SourceAsset;
		//Ref<Rendering::Material> m_Material;
	};
	struct AudioSourceComponent 
	{
		std::string SourceFile;
		//Audio::AudioSource Source;

		float Gain = 1.0f;
		float Pitch = 1.0f;

		bool Looping = false;
		bool Spatial = false;
		bool PlayOnCreate = false;
	};
}
