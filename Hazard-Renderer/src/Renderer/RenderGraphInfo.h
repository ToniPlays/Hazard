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
		friend class RenderGraph;

		uint32_t Iteration;
		bool Enabled;
		
		template<typename T>
		T& Data() const
		{
			return *(T*)m_UserData;
		}

		template<typename T>
		T& DrawData() const
		{
			return *(T*)m_DrawData;
		}

	private:
		void* m_UserData;
		void* m_DrawData;
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
		Callback<void(const RenderGraphFuncData&)> OnPrepare;
		Callback<void(const RenderGraphFuncData&)> OnFinished;
	};
}