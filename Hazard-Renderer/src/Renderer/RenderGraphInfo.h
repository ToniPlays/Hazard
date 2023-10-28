#pragma once

#include "Core/Rendering/Image2D.h"
#include "Core/Rendering/Sampler.h"
#include "Core/Rendering/GPUBuffer.h"
#include "Core/Rendering/Pipeline.h"
#include "Core/Rendering/DescriptorSet.h"
#include <span>

namespace HazardRenderer
{
		struct RenderGraphCreateInfo
	{
		std::string DebugName;
		uint64_t StageCount;
	};
}