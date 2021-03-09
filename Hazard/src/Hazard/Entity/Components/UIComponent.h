#pragma once

#include "Hazard/Math/Color.h"
#include "Hazard/Rendering/Textures/Texture.h"

namespace Hazard::ECS {

	//Quad sprite renderer
	struct SpriteRendererComponent {
		Color tint = Color::FromHex("#FFFFFFFF");
		Rendering::Texture* texture = nullptr;
	};
	//TODO: Remove batch component
	struct BatchComponent {
		Color tint;
		uint32_t size;
	};
}