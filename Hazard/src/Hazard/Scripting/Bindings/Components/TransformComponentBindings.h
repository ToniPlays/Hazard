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
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		return entity.GetComponent<TransformComponent>().GetTranslation();
	}
	static void TransformComponent_SetPosition_Native(uint64_t id, glm::vec3 position)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		entity.GetComponent<TransformComponent>().SetTranslation(position);
	}
	static glm::vec3 TransformComponent_GetRotation_Native(uint64_t id)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		return entity.GetComponent<TransformComponent>().GetRotationEuler();
	}
	static void TransformComponent_SetRotation_Native(uint64_t id, glm::vec3 rotation)
	{
		glm::vec3 radians = glm::radians(rotation);
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		entity.GetComponent<TransformComponent>().SetRotation(radians);
	}
	static glm::vec3 TransformComponent_GetScale_Native(uint64_t id)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		return entity.GetComponent<TransformComponent>().GetScale();
	}
	static void TransformComponent_SetScale_Native(uint64_t id, glm::vec3 scale)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		entity.GetComponent<TransformComponent>().SetScale(scale);
	}
}