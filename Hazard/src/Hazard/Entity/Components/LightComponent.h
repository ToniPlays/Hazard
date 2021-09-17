#pragma once

#include "Hazard/Math/Color.h"

namespace Hazard::ECS 
{
	struct SkyLightComponent 
	{
		Color m_Tint = Color::White;
		float m_Intensity = 1.0f;
	};

	struct DirectionalLightComponent 
	{
		Color m_Tint = Color::White;
		float m_Intensity = 1.0f;
	};

	struct PointLightComponent 
	{
		Color m_Tint = Color::White;
		float m_Intensity = 1.0f;
		float m_Radius = 10.0f;
	};
}