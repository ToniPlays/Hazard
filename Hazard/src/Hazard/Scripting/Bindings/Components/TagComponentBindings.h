#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../../ScriptEngine.h"
#include "../ECS/EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static Coral::String TagComponent_GetName_Native(uint64_t id)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		auto& tag = entity.GetComponent<TagComponent>();
		return Coral::String();
	}
	static void TagComponent_SetName_Native(uint64_t id, Coral::String tag)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		entity.GetTag().Tag = tag;
	}
}
