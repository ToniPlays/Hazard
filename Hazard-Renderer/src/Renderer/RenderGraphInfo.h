#pragma once

#include "Core/Rendering/Image2D.h"
#include "Core/Rendering/Sampler.h"
#include "Core/Rendering/GPUBuffer.h"
#include "Core/Rendering/Pipeline.h"
#include "Core/Rendering/DescriptorSet.h"
#include "Core/Rendering/RenderCommandBuffer.h"
#include <span>

namespace HazardRenderer
{
	struct RenderGraphStage
	{
		std::string DebugName;
		bool Enabled = true;
		uint32_t DataSize;

		std::function<void(Ref<RenderCommandBuffer>, void*)> Execute;
		std::function<void(Ref<RenderCommandBuffer>)> OnDisabled;
		std::function<void(Ref<RenderCommandBuffer>)> OnPrepare;
		std::function<void(Ref<RenderCommandBuffer>)> OnFinished;
	};

	struct RenderGraphCreateInfo
	{
		std::string DebugName;
		uint64_t StageCount;
		RenderGraphStage* pStages;
	};
}