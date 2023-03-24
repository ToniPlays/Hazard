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
		return mc.MeshHandle;
	}

	static void MeshComponent_SetMesh_Native(uint64_t id, uint64_t handle)
	{
		GET_ENTITY(id).GetComponent<MeshComponent>().MeshHandle = handle;
	}
}