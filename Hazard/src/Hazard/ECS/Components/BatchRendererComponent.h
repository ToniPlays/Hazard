#pragma once
#include "Hazard/ECS/Components/Component.h"

namespace Hazard {
	class BatchRendererComponent : public Component {
	public:
		BatchRendererComponent();
		~BatchRendererComponent();

		void OnAttach() override;
		void OnDetach() override;
		void OnRender() override;

		void SerializeComponent(YAML::Emitter& out);
		void DeserializeComponent(YAML::Node in);

		float size = 1;
		uint32_t textureIndex = 0;
	};
}