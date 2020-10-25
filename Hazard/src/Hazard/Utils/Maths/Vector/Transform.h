#pragma once

#include "Hazard/Core/Core.h"
#include "Vector3.h"
#include "Hazard/ECS/Components/Component.h"

#include <glm/glm.hpp>

namespace Hazard {

	class HAZARD_API Transform : public Component {

	public:
		Transform();
		Transform(Vector3<float> position, Vector3<float> rotation, Vector3<float> scale);
		static glm::mat4 AsMat4(Transform transform);
	public:
		Vector3<float> position, rotation, scale;
	};
}