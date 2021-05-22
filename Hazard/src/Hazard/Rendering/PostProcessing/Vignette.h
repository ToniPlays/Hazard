#pragma once

#include "Hazard/Core/Core.h"
#include "../Textures/RenderTexture.h"

namespace Hazard::Rendering {

	class VignetteEffect {
	public:
		virtual void Process(RenderTexture* source, glm::vec2 res) = 0;
		virtual RenderTexture& GetTargetTexture() = 0;
	};
}