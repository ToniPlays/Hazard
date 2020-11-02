#pragma once
#include <hzrpch.h>
#include "Transform.h"
#include "Hazard/Utils/Loaders/Serializer.h"
#include "Hazard/Utils/Loaders/Deserializer.h"

#include <glm/gtx/quaternion.hpp>

namespace Hazard {

	Transform::Transform(Vector3<float> position, Vector3<float> rotation, Vector3<float> scale) {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
	glm::mat4 Transform::AsMat4(Transform t)
	{
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), Vector3<float>::AsGLM(t.scale));
		return Transform::AsMat4NoScale(t) * scale;
	}

	glm::mat4 Transform::AsMat4NoScale(Transform t)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), Vector3<float>::AsGLM(t.position));
		glm::vec3 rotation = Vector3<float>::AsGLM(t.rotation);

		glm::quat rot = glm::quat(glm::vec3(glm::radians(rotation.z), glm::radians(rotation.y), glm::radians(rotation.x)));

		return transform * glm::mat4(rot);
	}
	
}