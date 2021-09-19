#pragma once

#include "Hazard/Math/Color.h"
#include "Hazard/Math/Math.h"
#include "Hazard/Core/UID.h"
#include "Hazard/Audio/AudioClip.h"

namespace Hazard::ECS {

	struct TagComponent
	{
		std::string m_Tag;
		UID m_ID;
		bool m_Visible = true;
	};

	struct TransformComponent
	{
		glm::vec3 m_Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 GetTransformMat4()
		{
			return Math::ToTransformMatrix(m_Translation, m_Rotation, m_Scale);
		}
		glm::mat4 GetTransformNoScale()
		{
			return Math::ToTransformMatrix(m_Translation, m_Rotation);
		}
	};

	struct ScriptComponent 
	{
		std::string m_ModuleName;
	};
	struct VisualScriptComponent 
	{
		std::string m_Filename;
	};
	struct MeshComponent 
	{
		float yes;
		//Ref<Rendering::Mesh> m_Mesh;
		//Ref<Rendering::Material> m_Material;
	};
	struct AudioSourceComponent 
	{
		std::string sourceFile;
		Audio::AudioClip source;
	};
}
