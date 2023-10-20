#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../../ScriptEngine.h"
#include "../ECS/EntityBindings.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardScript;

	static void SpriteRendererComponent_GetColor_Native(uint64_t id, glm::vec4* color)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		*color = entity.GetComponent<SpriteRendererComponent>().Color;
	}

	static void SpriteRendererComponent_SetColor_Native(uint64_t id, glm::vec4 color)
	{
		ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id).GetComponent<SpriteRendererComponent>().Color = Color::FromGLM(color);
	}
	static uint64_t SpriteRendererComponent_GetSprite_Native(uint64_t id)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		auto& src = entity.GetComponent<SpriteRendererComponent>();
		return src.TextureHandle;
	}
	static void SpriteRendererComponent_SetSprite_Native(uint64_t id, uint64_t handle)
	{
		auto entity = ScriptEngine::GetTargetWorldForEntity(id)->GetEntityFromUID(id);
		auto& src = entity.GetComponent<SpriteRendererComponent>();
		src.TextureHandle = handle;
	}
}