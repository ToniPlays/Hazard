#pragma once

#include <hzrpch.h>
#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Hazard {

	Transform::Transform() : Component("Transform") {
		this->position = Vector3<float>::zero;
		this->rotation = Vector3<float>::zero;
		this->scale = Vector3<float>::one;
	}

	Transform::Transform(Vector3<float> position, Vector3<float> rotation, Vector3<float> scale) : Component("Transform")
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}
	glm::mat4 Transform::AsMat4(Transform _transform)
	{
		glm::vec3 rotation = Vector3<float>::AsGLM(_transform.rotation);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), Vector3<float>::AsGLM(_transform.position))
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(0, 0, 1))
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0))
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(1, 0, 0))
			* glm::scale(glm::mat4(1.0f), Vector3<float>::AsGLM(_transform.scale));
		return transform;
	}
}