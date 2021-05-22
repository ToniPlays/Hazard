#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/PostProcessing/PostProcessingEffect.h"

namespace Hazard::Rendering {

	class VignetteEffect : public PostProcessingEffect {
	public:
		virtual void Process(FrameBuffer* source, glm::vec2 res) = 0;
		virtual FrameBuffer& GetTargetTexture() = 0;
	};
}