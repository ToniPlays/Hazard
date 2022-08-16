#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static float CameraComponent_GetFOV_Native(uint64_t id)
	{
		return GET_ENTITY(id).GetComponent<CameraComponent>().GetFov();
	}

	static void CameraComponent_SetFOV_Native(uint64_t id, float newFov)
	{
		GET_ENTITY(id).GetComponent<CameraComponent>().SetFov(newFov);
	}
}