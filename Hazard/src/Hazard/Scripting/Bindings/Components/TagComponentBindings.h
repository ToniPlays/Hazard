#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../../ScriptEngine.h"
#include "../ECS/EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static const CharType* TagComponent_GetName_Native(uint64_t id)
	{
		auto& tag = GET_ENTITY(id).GetComponent<TagComponent>();
		auto str = Coral::StringHelper::ConvertUtf8ToWide(tag.Tag);
		return str.c_str();
	}
	static void TagComponent_SetName_Native(uint64_t id, const CharType* tag)
	{
		GET_ENTITY(id).GetTag().Tag = Coral::StringHelper::ConvertWideToUtf8(tag);
	}
}