#pragma once

#include "Hazard/Math/Color.h"

/*
Light emitting components
*/
namespace Hazard::ECS {
	//Skylight emits ambient light
	struct SkyLightComponent {
		float m_Intensity = 1.0f;
		Color m_Tint;
	};

	//Represents sun
	struct DirectionalLightComponent {
		float m_Intensity = 1.0f;
		Color m_Tint;
	};

	//Point light
	struct PointLightComponent {
		float m_Intensity = 1.0f;
		Color m_Tint;

		float m_Radius = 10.0f;
	};
}