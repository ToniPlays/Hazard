#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/PostProcessing/PostProcessingEffect.h"

namespace Hazard::Rendering {

	class GaussianBlurEffect : public PostProcessingEffect {
	public:
		virtual ~GaussianBlurEffect() {};
	};
}