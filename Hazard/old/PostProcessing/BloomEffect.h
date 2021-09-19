#pragma once

#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/PostProcessing/PostProcessingEffect.h"

namespace Hazard::Rendering {

	class BloomEffect : public PostProcessingEffect {
	public:
		BloomEffect() = default;
		virtual ~BloomEffect() = default;
	public:
		float threshold;
		float intensity;
	};
}