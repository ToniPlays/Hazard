#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "Hazard/ECS/WorldHandler.h"
#include "EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static float SkyLightComponent_GetIntensity_Native(uint64_t handle)
	{
		return GET_ENTITY(handle).GetComponent<SkyLightComponent>().Intensity;
	}
	static void SkyLightComponent_SetIntensity_Native(uint64_t handle, float intensity)
	{
		GET_ENTITY(handle).GetComponent<SkyLightComponent>().Intensity = intensity;
	}
	static uint64_t SkyLightComponent_GetEnvironmentMap_Native(uint64_t handle)
	{
		auto& slc = GET_ENTITY(handle).GetComponent<SkyLightComponent>();
		const Ref<EnvironmentMap>& map = slc.EnvironmentMap;

		if (!slc.EnvironmentMap) 
			return 0;
		return slc.EnvironmentMap->GetHandle();
	}
	static void SkyLightComponent_SetEnvironmentMap_Native(uint64_t handle, uint64_t assetHandle)
	{
		
	}
}