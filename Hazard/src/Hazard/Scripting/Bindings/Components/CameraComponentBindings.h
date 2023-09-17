#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../../ScriptEngine.h"
#include "../ECS/EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static int CameraComponent_GetProjection_Native(uint64_t id)
	{
		return (int)GET_ENTITY(id).GetComponent<CameraComponent>().GetProjectionType();
	}
	static void CameraComponent_SetProjection_Native(uint64_t id, int projection)
	{
		GET_ENTITY(id).GetComponent<CameraComponent>().SetProjection((Projection)projection);
	}

	static float CameraComponent_GetFOV_Native(uint64_t id)
	{
		return GET_ENTITY(id).GetComponent<CameraComponent>().GetFov();
	}
	static void CameraComponent_SetFOV_Native(uint64_t id, float newFov)
	{
		GET_ENTITY(id).GetComponent<CameraComponent>().SetFov(newFov);
	}

	static float CameraComponent_GetOrthoSize_Native(uint64_t id)
	{
		return GET_ENTITY(id).GetComponent<CameraComponent>().GetSize();
	}
	static void CameraComponent_SetOrthoSize_Native(uint64_t id, float size)
	{
		GET_ENTITY(id).GetComponent<CameraComponent>().SetSize(size);
	}

	static void CameraComponent_GetClipping_Native(uint64_t id, glm::vec2* clipping)
	{
		*clipping = GET_ENTITY(id).GetComponent<CameraComponent>().GetClipping();
	}
	static void CameraComponent_SetClipping_Native(uint64_t id, float zNear, float zFar)
	{
		GET_ENTITY(id).GetComponent<CameraComponent>().SetClipping({ zNear, zFar });
	}
}