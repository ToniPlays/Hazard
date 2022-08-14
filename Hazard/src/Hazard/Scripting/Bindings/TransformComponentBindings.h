#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static void Transform_GetPosition_Native(uint64_t id, glm::vec3* position)
	{
		*position = GET_ENTITY(id).GetComponent<TransformComponent>().Translation;
	}

	static void Transform_SetPosition_Native(uint64_t id, glm::vec3 position)
	{
		GET_ENTITY(id).GetComponent<TransformComponent>().Translation = position;
	}

	static void Transform_GetRotation_Native(uint64_t id, glm::vec3* rotation)
	{
		*rotation = GET_ENTITY(id).GetComponent<TransformComponent>().Rotation;
	}
	static void Transform_SetRotation_Native(uint64_t id, glm::vec3 rotation)
	{
		GET_ENTITY(id).GetComponent<TransformComponent>().Rotation = rotation;
	}
	static void Transform_GetScale_Native(uint64_t id, glm::vec3* scale)
	{
		*scale = GET_ENTITY(id).GetComponent<TransformComponent>().Scale;
	}

	static void Transform_SetScale_Native(uint64_t id, glm::vec3 scale)
	{
		GET_ENTITY(id).GetComponent<TransformComponent>().Scale = scale;
	}
}