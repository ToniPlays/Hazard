#pragma once

#include "Color.h"
#include "Hazard/Rendering/Texture2D.h"
#include "HazardRendererCore.h"

namespace Hazard {

	struct SpriteRendererComponent : ComponentBase
	{
		Color Color = Color::White;
		Ref<Texture2D> Texture;

		SpriteRendererComponent() = default;
	};

	struct BatchComponent : ComponentBase
	{
		Color Tint = Color::White;
		uint32_t Size = 0;

		BatchComponent() = default;
	};
}