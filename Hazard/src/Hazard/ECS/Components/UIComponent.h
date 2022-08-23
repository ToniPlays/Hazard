#pragma once

#include "Color.h"
#include "Hazard/Rendering/Texture2D.h"
#include "Hazard/Core/Core.h"
#include "HazardRendererCore.h"

namespace Hazard {

	struct SpriteRendererComponent : ComponentBase
	{
		Color Color = Color::White;
		Ref<Texture2D> Texture;

		SpriteRendererComponent() = default;
	};
}