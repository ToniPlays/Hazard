#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../../ScriptEngine.h"
#include "Hazard/ECS/WorldHandler.h"
#include "../ECS/EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static float SkyLightComponent_GetIntensity_Native(uint64_t handle)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(handle)->GetEntityFromUID(handle);
		return entity.GetComponent<SkyLightComponent>().Intensity;
	}
	static void SkyLightComponent_SetIntensity_Native(uint64_t handle, float intensity)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(handle)->GetEntityFromUID(handle);
		entity.GetComponent<SkyLightComponent>().Intensity = intensity;
	}
	static uint64_t SkyLightComponent_GetEnvironmentMap_Native(uint64_t handle)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(handle)->GetEntityFromUID(handle);
		auto& slc = entity.GetComponent<SkyLightComponent>();
		//TODO: Fix this
		return INVALID_ASSET_HANDLE;
	}
	static void SkyLightComponent_SetEnvironmentMap_Native(uint64_t handle, uint64_t assetHandle)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(handle)->GetEntityFromUID(handle);
		auto& slc = entity.GetComponent<SkyLightComponent>();
		slc.EnvironmentMapHandle = assetHandle;
	}
}