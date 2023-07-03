#pragma once

#include "Color.h"
#include "Hazard/RenderContext/Texture2D.h"
#include "Hazard/Core/Core.h"
#include "HazardRendererCore.h"

namespace Hazard {

	struct SpriteRendererComponent : ComponentBase
	{
		Color Color = Color::White;
		AssetHandle TextureHandle = INVALID_ASSET_HANDLE;

		SpriteRendererComponent() = default;
	};
}