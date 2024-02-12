#pragma once

#include "Core/Rendering/Image2D.h"
#include "Core/Rendering/Sampler.h"
#include "Core/Rendering/GPUBuffer.h"
#include "Core/Rendering/Pipeline.h"
#include "Core/Rendering/DescriptorSet.h"
#include "Core/Rendering/RenderCommandBuffer.h"
#include "Callback.h"

namespace HazardRenderer
{
	struct RenderGraphFuncData
	{
		Ref<RenderCommandBuffer> CommandBuffer;
		Ref<RenderPass> CurrentRenderPass;
		bool Enabled;
		void* Data;
	};
	struct RenderGraphStage
	{
		std::string DebugName;
		uint32_t Stride;
		bool Enabled = true;

		Callback<void(const RenderGraphFuncData&)> Execute;
		Callback<void(const RenderGraphFuncData&)> OnPrepare;
		Callback<void(const RenderGraphFuncData&)> OnFinished;
	};

	struct RenderGraphCreateInfo
	{
		std::string DebugName;
		uint64_t StageCount;
		RenderGraphStage* pStages;
	};
}