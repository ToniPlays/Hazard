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

	static uint64_t EnvironmentMap_Create_Native()
	{
		Ref<EnvironmentMap> map = Ref<EnvironmentMap>::Create();
		//AssetManager::CreateMemoryOnly(AssetType::EnvironmentMap, map);
		return map->GetHandle();
	}
	static void EnvironmentMap_Destroy_Native(uint64_t handle)
	{
		Ref<EnvironmentMap> map = AssetManager::GetAsset<EnvironmentMap>(handle);
	}

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
		Ref<AssetPointer> cubeMap = AssetManager::GetAsset<AssetPointer>(assetHandle);

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
	static void EnvironmentMap_GenerateRadiance_Native(uint64_t handle, uint64_t sourceImage)
	{
		Ref<EnvironmentMap> map = AssetManager::GetAsset<EnvironmentMap>(handle);
		Ref<Texture2DAsset> source = AssetManager::GetAsset<Texture2DAsset>(sourceImage);
		map->GenerateRadiance();
	}
	static void EnvironmentMap_GenerateIrradiance_Native(uint64_t handle, uint64_t sourceCube)
	{
		Ref<EnvironmentMap> map = AssetManager::GetAsset<EnvironmentMap>(handle);
		Ref<AssetPointer> source = AssetManager::GetAsset<AssetPointer>(sourceCube);
		map->GenerateIrradiance(source);
	}
	static void EnvironmentMap_GeneratePrefilter_Native(uint64_t handle, uint64_t sourceCube)
	{
		Ref<EnvironmentMap> map = AssetManager::GetAsset<EnvironmentMap>(handle);
		Ref<AssetPointer> source = AssetManager::GetAsset<AssetPointer>(sourceCube);
		map->GeneratePreFilter(source);
	}
}