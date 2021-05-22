#pragma once

#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/PostProcessing/PostProcessingEffect.h"

namespace Hazard::Rendering {

	class BloomEffect : public PostProcessingEffect {
	public:
		virtual ~BloomEffect() {};
	public:
		float threshold;
		float intensity;
	};
}