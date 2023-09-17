#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../../ScriptEngine.h"
#include "../ECS/EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static glm::vec3 TransformComponent_GetPosition_Native(uint64_t id)
	{
		return GET_ENTITY(id).GetComponent<TransformComponent>().GetTranslation();
	}
	static void TransformComponent_SetPosition_Native(uint64_t id, glm::vec3 position)
	{
		GET_ENTITY(id).GetComponent<TransformComponent>().SetTranslation(position);
	}
	static glm::vec3 TransformComponent_GetRotation_Native(uint64_t id)
	{
		 return GET_ENTITY(id).GetComponent<TransformComponent>().GetRotationEuler();
	}
	static void TransformComponent_SetRotation_Native(uint64_t id, glm::vec3 rotation)
	{
		glm::vec3 radians = glm::radians(rotation);
		GET_ENTITY(id).GetComponent<TransformComponent>().SetRotation(radians);
	}
	static glm::vec3 TransformComponent_GetScale_Native(uint64_t id)
	{
		return GET_ENTITY(id).GetComponent<TransformComponent>().GetScale();
	}
	static void TransformComponent_SetScale_Native(uint64_t id, glm::vec3 scale)
	{
		GET_ENTITY(id).GetComponent<TransformComponent>().SetScale(scale);
	}
}