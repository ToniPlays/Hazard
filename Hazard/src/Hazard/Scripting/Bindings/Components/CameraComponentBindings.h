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
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		return (int)entity.GetComponent<CameraComponent>().GetProjectionType();
	}
	static void CameraComponent_SetProjection_Native(uint64_t id, int projection)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		entity.GetComponent<CameraComponent>().SetProjection((Projection)projection);
	}

	static float CameraComponent_GetFOV_Native(uint64_t id)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		return entity.GetComponent<CameraComponent>().GetFov();
	}
	static void CameraComponent_SetFOV_Native(uint64_t id, float newFov)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		entity.GetComponent<CameraComponent>().SetFov(newFov);
	}

	static float CameraComponent_GetOrthoSize_Native(uint64_t id)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		return entity.GetComponent<CameraComponent>().GetSize();
	}
	static void CameraComponent_SetOrthoSize_Native(uint64_t id, float size)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		entity.GetComponent<CameraComponent>().SetSize(size);
	}

	static void CameraComponent_GetClipping_Native(uint64_t id, glm::vec2* clipping)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		*clipping = entity.GetComponent<CameraComponent>().GetClipping();
	}
	static void CameraComponent_SetClipping_Native(uint64_t id, float zNear, float zFar)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		entity.GetComponent<CameraComponent>().SetClipping({ zNear, zFar });
	}
}