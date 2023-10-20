#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../../ScriptEngine.h"
#include "../ECS/EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static Coral::NativeString TagComponent_GetName_Native(uint64_t id)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		auto& tag = entity.GetComponent<TagComponent>();
		return Coral::NativeString::FromUTF8(tag.Tag);
	}
	static void TagComponent_SetName_Native(uint64_t id, Coral::NativeString tag)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		entity.GetTag().Tag = tag.ToString();
	}
}