#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Utils/Maths/Vector/Vector3.h"
#include "Hazard/ECS/Components/Component.h"

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace Hazard {

	class HAZARD_API Transform : public Component {

	public:
		Transform();
		Transform(Vector3<float> position, Vector3<float> rotation, Vector3<float> scale);
		static glm::mat4 AsMat4(Transform transform);

		void SerializeComponent(YAML::Emitter& out) override;
		void DeserializeComponent(YAML::Node in) override;
	public:
		Vector3<float> position, rotation, scale;
	};
}