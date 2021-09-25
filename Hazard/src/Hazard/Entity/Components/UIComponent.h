#pragma once

#include "Hazard/Math/Color.h"
#include "Hazard/Rendering/Texture/Texture2D.h"

namespace Hazard::ECS {

	struct SpriteRendererComponent {
		Color m_Tint = Color::White;
		Ref<Rendering::Texture2D> m_Texture;

		SpriteRendererComponent() = default;
	};

	struct BatchComponent {
		Color m_Tint = Color::White;
		uint32_t m_Size = 0;

		BatchComponent() = default;
	};
}