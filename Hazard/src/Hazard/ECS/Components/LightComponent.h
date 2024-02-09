#pragma once

#include "Color.h"
#include "ComponentBase.h"
#include "Hazard/Rendering/Environment/EnvironmentMap.h"

namespace Hazard 
{
	struct SkyLightComponent : ComponentBase
	{
		AssetHandle EnvironmentMapHandle = INVALID_ASSET_HANDLE;
		float Intensity = 1.0f;
		float LodLevel = 0.0f;
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