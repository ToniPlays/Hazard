#pragma once

#include "Hazard/Math/Color.h"
#include "Hazard/Rendering/Textures/Texture.h"

namespace Hazard::ECS {

	struct SpriteRendererComponent {
		Color m_Tint = Color::FromHex("#FFFFFFFF");
		Rendering::Texture* m_Texture = nullptr;
	};

	struct BatchComponent {
		Color m_Tint;
		uint32_t m_Size;
	};
}