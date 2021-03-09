#pragma once

#include "Hazard/Math/Color.h"

/*
Light emitting components
*/
namespace Hazard::ECS {
	//Skylight emits ambient light
	struct SkyLightComponent {
		float intensity = 1.0f;
		Color tint;
	};
	//Represents sun
	struct DirectionalLightComponent {
		float intensity = 1.0f;
		Color tint;
	};
	//Point light
	struct PointLightComponent {
		float intensity = 1.0f;
		Color tint;

		float radius = 10.0f;
	};
}