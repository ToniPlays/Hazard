#pragma once

#include "Hazard/Math/Color.h"

namespace Hazard::ECS 
{
	struct SkyLightComponent {
		float m_Intensity = 1.0f;
		Color m_Tint;
	};

	struct DirectionalLightComponent {
		float m_Intensity = 1.0f;
		Color m_Tint;
	};

	struct PointLightComponent {
		float m_Intensity = 1.0f;
		Color m_Tint;

		float m_Radius = 10.0f;
	};
}