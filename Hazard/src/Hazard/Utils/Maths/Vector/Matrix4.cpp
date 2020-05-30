#pragma once
#include <hzrpch.h>
#include "Matrix4.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Hazard {


	glm::mat4 Matrix4::GetModelMatrix(Transform transform)
	{
		glm::mat4 mat = glm::translate(glm::mat4(1.0f), glm::vec3(transform.position.x, transform.position.y, transform.position.z));
		mat = glm::rotate(mat, (float)Math::ToRadians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		mat = glm::rotate(mat, (float)Math::ToRadians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		mat = glm::rotate(mat, (float)Math::ToRadians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		mat = glm::scale(mat, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));
		return mat;
	}
}