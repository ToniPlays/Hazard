#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static MonoString* Tag_GetName_Native(uint64_t id) {
		auto& tag = GET_ENTITY(id).GetComponent<TagComponent>();
		return Mono::StringToMonoString(tag.Tag);
	}
	static void Tag_SetName_Native(uint64_t id, MonoString* tag) {
		GET_ENTITY(id).GetTag().Tag = Mono::MonoStringToString(tag);
	}
}