#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	enum class EnvironmentResource
	{
		RadianceMap = 0,
		IrradianceMap = 1,
		PreFilterMap = 2,
		BRDFLut = 3
	};

	using namespace HazardScript;

	static uint64_t EnvironmentMap_GetCubemapTexture_Native(uint64_t handle, int type)
	{
		Ref<EnvironmentMap> env = AssetManager::GetAsset<EnvironmentMap>(handle);
		if (!env) return 0;

		switch ((EnvironmentResource)type)
		{
		case EnvironmentResource::RadianceMap:
		{
			if (!env->RadianceMap) return 0;
			return env->RadianceMap->GetHandle();
		}
		case EnvironmentResource::IrradianceMap:
		{
			if (!env->IrradianceMap) return 0;
			return env->IrradianceMap->GetHandle();
		}
		case EnvironmentResource::PreFilterMap:
		{
			if (!env->PreFilterMap) return 0;
			return env->PreFilterMap->GetHandle();
		}
		}
		return 0;
	}
	static void EnvironmentMap_SetCubemapTexture_Native(uint64_t handle, int type, uint64_t assetHandle)
	{
		Ref<EnvironmentMap> env = AssetManager::GetAsset<EnvironmentMap>(handle);
		Ref<HazardRenderer::CubemapTexture> cubeMap = AssetManager::GetRuntimeAsset<HazardRenderer::CubemapTexture>(assetHandle);
		return;

		switch ((EnvironmentResource)type)
		{
		case EnvironmentResource::RadianceMap:
		{
			env->RadianceMap = cubeMap;
			break;
		}
		case EnvironmentResource::IrradianceMap:
		{
			env->IrradianceMap = cubeMap;
			break;
		}
		case EnvironmentResource::PreFilterMap:
		{
			env->PreFilterMap = cubeMap;
			break;
		}
		}
	}
}