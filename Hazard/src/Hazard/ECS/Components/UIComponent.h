#pragma once

#include "Color.h"

namespace Hazard {

	struct SpriteRendererComponent : ComponentBase
	{
		Color Tint = Color::White;
		//Ref<Rendering::Texture2D> Texture;

		SpriteRendererComponent() = default;
	};

	struct BatchComponent : ComponentBase
	{
		Color Tint = Color::White;
		uint32_t Size = 0;

		BatchComponent() = default;
	};
}