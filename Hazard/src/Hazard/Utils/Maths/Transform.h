#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Vector/Vector3.h"
#include "Vector/Matrix4.h"
#include "Hazard/ECS/Components/Component.h"
#include <yaml-cpp/yaml.h>

namespace Hazard {
	
	class Transform {
	public:
		Vector3<float> position, rotation, scale;

	public:
		Transform(Vector3<float> position = Vector3<float>::zero, Vector3<float> rotation = Vector3<float>::zero, Vector3<float> scale = Vector3<float>::one);

		static glm::mat4 Transform::AsMat4(Transform t);
		static glm::mat4 Transform::AsMat4NoScale(Transform t);
		
	};
}

