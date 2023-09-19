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
		auto& tag = GET_ENTITY(id).GetComponent<TagComponent>();
		return Coral::NativeString::FromUTF8(tag.Tag);
	}
	static void TagComponent_SetName_Native(uint64_t id, Coral::NativeString tag)
	{
		GET_ENTITY(id).GetTag().Tag = tag.ToString();
	}
}