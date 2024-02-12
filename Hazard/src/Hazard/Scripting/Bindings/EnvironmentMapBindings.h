#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardScript;

	static uint64_t EnvironmentMap_Create_Native()
	{
		return INVALID_ASSET_HANDLE;
	}
	static void EnvironmentMap_Destroy_Native(uint64_t handle)
	{

	}

	static uint64_t EnvironmentMap_GetCubemapTexture_Native(uint64_t handle, int type)
	{
		return INVALID_ASSET_HANDLE;
	}
	static void EnvironmentMap_SetCubemapTexture_Native(uint64_t handle, int type, uint64_t assetHandle)
	{

	}
	static void EnvironmentMap_GenerateRadiance_Native(uint64_t handle, uint64_t sourceImage)
	{

	}
	static void EnvironmentMap_GenerateIrradiance_Native(uint64_t handle, uint64_t sourceCube)
	{

	}
	static void EnvironmentMap_GeneratePrefilter_Native(uint64_t handle, uint64_t sourceCube)
	{

	}
}