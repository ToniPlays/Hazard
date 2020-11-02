#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Utils/Maths/Transform.h"
#include "Hazard/ECS/Components/Component.h"
#include <yaml-cpp/yaml.h>

namespace Hazard {

	class HAZARD_API TransformComponent : public Component {

	public:
		TransformComponent();
		~TransformComponent();

		void SerializeComponent(YAML::Emitter& out) override;
		void DeserializeComponent(YAML::Node in) override;

		Transform transform;
	};
}