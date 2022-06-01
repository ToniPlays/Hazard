#pragma once

#include "Color.h"

namespace Hazard::ECS {

	struct SpriteRendererComponent 
	{
		Color Tint = Color::White;
		//Ref<Rendering::Texture2D> Texture;

		SpriteRendererComponent() = default;
	};

	struct BatchComponent {
		Color Tint = Color::White;
		uint32_t Size = 0;

		BatchComponent() = default;
	};
}