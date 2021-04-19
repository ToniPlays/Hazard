#pragma once
#include <hzrpch.h>
#include "TransformBindings.h"
#include "Hazard/Scripting/ScriptCommand.h"
#include "../../ScriptUtils.h"


namespace Hazard::Scripting::CSharp::Bindings {

	BindMap TransformBindings::GetBindings()
	{
		BindMap result;
		result.emplace_back("Hazard.TransformComponent::Transform_GetPosition_Native", Transform_GetPosition_Native);
		result.emplace_back("Hazard.TransformComponent::Transform_SetPosition_Native", Transform_SetPosition_Native);

		result.emplace_back("Hazard.TransformComponent::Transform_GetRotation_Native", Transform_GetRotation_Native);
		result.emplace_back("Hazard.TransformComponent::Transform_SetRotation_Native", Transform_SetRotation_Native);

		result.emplace_back("Hazard.TransformComponent::Transform_GetScale_Native", Transform_GetScale_Native);
		result.emplace_back("Hazard.TransformComponent::Transform_SetScale_Native", Transform_SetScale_Native);
		return result;
	}
	void TransformBindings::Transform_GetPosition_Native(uint32_t id, glm::vec3* position)
	{
		
		auto& c = ScriptCommand::EntityGetComponent<ECS::TransformComponent>(id);
		HZR_CORE_INFO("Getting position {0} {1} {2}", position->x, position->y, position->z);
		*position = c.Translation;
	}
	void TransformBindings::Transform_SetPosition_Native(uint32_t id, glm::vec3* position)
	{
		auto& c = ScriptCommand::EntityGetComponent<ECS::TransformComponent>(id);
		HZR_CORE_INFO("Setting position {0} {1} {2}", position->x, position->y, position->z);
		c.Translation = *position;
	}
	void TransformBindings::Transform_GetRotation_Native(uint32_t id, glm::vec3* rotation)
	{
		auto& c = ScriptCommand::EntityGetComponent<ECS::TransformComponent>(id);
		*rotation = c.Rotation;
	}
	void TransformBindings::Transform_SetRotation_Native(uint32_t id, glm::vec3* rotation)
	{
		auto& c = ScriptCommand::EntityGetComponent<ECS::TransformComponent>(id);
		c.Rotation = *rotation;
	}
	void TransformBindings::Transform_GetScale_Native(uint32_t id, glm::vec3* scale)
	{
		auto& c = ScriptCommand::EntityGetComponent<ECS::TransformComponent>(id);
		*scale = c.Scale;
	}
	void TransformBindings::Transform_SetScale_Native(uint32_t id, glm::vec3* scale)
	{
		auto& c = ScriptCommand::EntityGetComponent<ECS::TransformComponent>(id);
		c.Scale = *scale;
	}
}