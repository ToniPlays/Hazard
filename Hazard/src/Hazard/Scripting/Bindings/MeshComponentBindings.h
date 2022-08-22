#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "Hazard/Assets/AssetManager.h"
#include "EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static uint64_t MeshComponent_GetMesh_Native(uint64_t id)
	{
		auto& mc = GET_ENTITY(id).GetComponent<MeshComponent>();
		return mc.m_MeshHandle ? mc.m_MeshHandle->GetHandle() : INVALID_ASSET_HANDLE;
	}

	static void MeshComponent_SetMesh_Native(uint64_t id, uint64_t handle)
	{
		Ref<Mesh>& asset = AssetManager::GetAsset<Mesh>(handle);
		if (asset == nullptr) {
			asset = AssetManager::GetRuntimeAsset<Mesh>(handle);
		}
		GET_ENTITY(id).GetComponent<MeshComponent>().m_MeshHandle = asset;
	}
}