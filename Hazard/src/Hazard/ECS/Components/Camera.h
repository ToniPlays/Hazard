#pragma once
#include "Component.h"

namespace Hazard {
	class HAZARD_API Camera : public Component {
	public:
		Camera();
		void SerializeComponent(YAML::Emitter& out) override;
		void DeserializeComponent(YAML::Node in) override;
	};
}