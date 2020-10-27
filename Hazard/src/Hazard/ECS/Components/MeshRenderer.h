#pragma once

#include "Hazard/Core/Core.h"


namespace Hazard {
	class HAZARD_API MeshRenderer : public Component {
	public:
		MeshRenderer();
		~MeshRenderer();

		void SerializeComponent(YAML::Emitter& out) override;
		void DeserializeComponent(YAML::Node in) override;

	private:
	};
}