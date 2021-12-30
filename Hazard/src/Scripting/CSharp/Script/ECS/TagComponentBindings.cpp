#pragma once
#include <hzrpch.h>
#include "TagComponentBindings.h"
#include "EntityBindings.h"
#include "Hazard/Scripting/ScriptCommand.h"
#include "../../ScriptUtils.h"


namespace Hazard::Scripting::CSharp::Bindings {

	BindMap TagComponentBindings::GetBindings()
	{
		BindMap result;
		result.emplace_back("Hazard.TagComponent::TagComponent_GetName_Native", (void*) TagComponent_GetName_Native);
		result.emplace_back("Hazard.TagComponent::TagComponent_SetName_Native", (void*) TagComponent_SetName_Native);
		return result;
	}
	MonoString* TagComponentBindings::TagComponent_GetName_Native(uint32_t id)
	{
		auto& tag = ScriptCommand::EntityGetComponent<ECS::TagComponent>(id);
		return ScriptUtils::StringToMonoString(tag.Tag.c_str());
	}
	void TagComponentBindings::TagComponent_SetName_Native(uint32_t id, MonoString* newName)
	{
		auto& tag = ScriptCommand::EntityGetComponent<ECS::TagComponent>(id);
		tag.Tag = ScriptUtils::MonoStringToString(newName);
	}
}