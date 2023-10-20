#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../../ScriptEngine.h"
#include "../ECS/EntityBindings.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardScript;

	static uint64_t MeshComponent_GetMesh_Native(uint64_t id)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		auto& mc = entity.GetComponent<MeshComponent>();
		return mc.MeshHandle;
	}

	static void MeshComponent_SetMesh_Native(uint64_t id, uint64_t handle)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		entity.GetComponent<MeshComponent>().MeshHandle = handle;
	}
}