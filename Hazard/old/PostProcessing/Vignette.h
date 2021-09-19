#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/PostProcessing/PostProcessingEffect.h"

namespace Hazard::Rendering {

	class VignetteEffect : public PostProcessingEffect {
	public:
		virtual ~VignetteEffect() {};
	public:
		float m_Outer, m_Inner, m_Intensity;
	};
}