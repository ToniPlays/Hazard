#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "Hazard/Assets/AssetManager.h"
#include "EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static void MeshComponent_SetMesh_Native(uint64_t id, uint64_t handle)
	{
		Ref<Mesh>& asset = AssetManager::GetAsset<Mesh>(handle);
		GET_ENTITY(id).GetComponent<MeshComponent>().m_MeshHandle = asset;
	}
}