#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "EntityBindings.h"

namespace Hazard
{
	using namespace HazardScript;

	static void SpriteRendererComponent_GetColor_Native(uint64_t id, glm::vec4* color)
	{
		*color = GET_ENTITY(id).GetComponent<SpriteRendererComponent>().Color;
	}

	static void SpriteRendererComponent_SetColor_Native(uint64_t id, glm::vec4 color)
	{
		GET_ENTITY(id).GetComponent<SpriteRendererComponent>().Color = Color::FromGLM(color);
	}
}