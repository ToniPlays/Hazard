#pragma once

#include "Color.h"
#include "ComponentBase.h"

namespace Hazard 
{
	struct SkyLightComponent : ComponentBase
	{
		Color Tint = Color::White;
		float Intensity = 1.0f;
	};

	struct DirectionalLightComponent : ComponentBase
	{
		Color Tint = Color::White;
		float Intensity = 1.0f;
	};

	struct PointLightComponent : ComponentBase
	{
		Color Tint = Color::White;
		float Intensity = 1.0f;
		float Radius = 10.0f;
	};
}