#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static void TransformComponent_GetPosition_Native(uint64_t id, glm::vec3* position)
	{
		*position = GET_ENTITY(id).GetComponent<TransformComponent>().GetTranslation();
	}

	static void TransformComponent_SetPosition_Native(uint64_t id, glm::vec3 position)
	{
		GET_ENTITY(id).GetComponent<TransformComponent>().SetTranslation(position);
	}

	static void TransformComponent_GetRotation_Native(uint64_t id, glm::vec3* rotation)
	{
		*rotation = glm::degrees(GET_ENTITY(id).GetComponent<TransformComponent>().GetRotation());
	}
	static void TransformComponent_SetRotation_Native(uint64_t id, glm::vec3 rotation)
	{
		GET_ENTITY(id).GetComponent<TransformComponent>().SetRotation(glm::radians(rotation));
	}
	static void TransformComponent_GetScale_Native(uint64_t id, glm::vec3* scale)
	{
		*scale = GET_ENTITY(id).GetComponent<TransformComponent>().GetScale();
	}

	static void TransformComponent_SetScale_Native(uint64_t id, glm::vec3 scale)
	{
		GET_ENTITY(id).GetComponent<TransformComponent>().SetScale(scale);
	}
}