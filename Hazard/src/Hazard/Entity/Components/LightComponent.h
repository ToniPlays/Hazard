#pragma once

#include "Hazard/Math/Color.h"

namespace Hazard::ECS {

	struct SkyLightComponent {
		float intensity = 1.0f;
		Color tint;
	};

	struct DirectionalLightComponent {
		float intensity = 1.0f;
		Color tint;
	};
	struct PointLightComponent {
		float intensity = 1.0f;
		Color tint;

		float radius = 10.0f;
	};
}