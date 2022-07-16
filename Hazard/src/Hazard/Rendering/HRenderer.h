#pragma once

#include "Hazard/ECS/Component.h"
#include "HazardRenderer.h"
#include "RenderEngine.h"
#include "WorldRenderer.h"

namespace Hazard 
{
	class HRenderer 
	{
		friend class RenderEngine;
	public:
		static void SubmitWorldRenderer(WorldRenderer* renderer);
		static void SubmitSprite(const TransformComponent& transform, const SpriteRendererComponent& spriteRenderer);
	private:
		inline static RenderEngine* s_Engine;
	};
}