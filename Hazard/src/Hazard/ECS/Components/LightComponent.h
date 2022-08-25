#pragma once

#include "Color.h"
#include "ComponentBase.h"
#include "Hazard/Rendering/Environment/EnvironmentMap.h"

namespace Hazard 
{
	struct SkyLightComponent : ComponentBase
	{
		Ref<EnvironmentMap> EnvironmentMap;
		Color LightColor = Color::White;
		float Intensity = 1.0f;
	};

	struct DirectionalLightComponent : ComponentBase
	{
		Color LightColor = Color::White;
		float Intensity = 1.0f;
	};

	struct PointLightComponent : ComponentBase
	{
		Color LightColor = Color::White;
		float Intensity = 1.0f;
		float Radius = 10.0f;
	};
}