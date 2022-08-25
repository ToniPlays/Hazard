#pragma once

#include "HazardRendererCore.h"

namespace HazardRenderer
{
	struct ComputePipelineCreateInfo
	{
		std::string DebugName;
		std::string ShaderPath;
		PipelineUsage Usage = PipelineUsage::None;
	};

	class ComputePipeline : public HZR_RESOURCE_HANDLER {
	public:
		virtual ~ComputePipeline() = default;

		virtual void Bind(Ref<RenderCommandBuffer> commandBuffer) = 0;
		virtual void Execute(Ref<RenderCommandBuffer> commandBuffer) = 0;

		virtual Ref<Shader> GetShader() = 0;

		static Ref<ComputePipeline> Create(ComputePipelineCreateInfo* createInfo);
	};
}