#pragma once

#include "Color.h"

namespace Hazard::ECS 
{
	struct SkyLightComponent 
	{
		Color Tint = Color::White;
		float Intensity = 1.0f;
	};

	struct DirectionalLightComponent 
	{
		Color Tint = Color::White;
		float Intensity = 1.0f;
	};

	struct PointLightComponent 
	{
		Color Tint = Color::White;
		float Intensity = 1.0f;
		float Radius = 10.0f;
	};
}