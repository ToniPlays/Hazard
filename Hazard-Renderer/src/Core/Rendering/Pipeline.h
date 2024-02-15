#pragma once

#include "BufferLayout.h"
#include "RenderPass.h"
#include "GPUBuffer.h"
#include "Shader.h"
#include "DescriptorSetLayout.h"

namespace HazardRenderer
{
	enum class PipelineUsage 
	{
		None = 0,
		GraphicsBit,
		ComputeBit,
		Raygen
	};
	enum PipelineFlags 
	{
		PIPELINE_DRAW_FILL = BIT(0),
		PIPELINE_DRAW_LINE = BIT(1),
		PIPELINE_DRAW_POINT = BIT(2),
		PIPELINE_CULL_FRONT_FACE = BIT(3),
		PIPELINE_CULL_BACK_FACE = BIT(4),
		PIPELINE_DEPTH_TEST = BIT(5),
		PIPELINE_DEPTH_WRITE = BIT(6),
		PIPELINE_PRIMITIVE_TOPOLOGY_POINT_LIST = BIT(7),
		PIPELINE_PRIMITIVE_TOPOLOGY_LINE_LIST = BIT(8),
		PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST = BIT(9),
	};
	enum class DepthOp 
	{
		None = 0,
		Never,
		NotEqual,
		Less,
		LessOrEqual,
		Greater,
		GreaterOrEqual,
		Equal,
		Always
	};

	struct PushConstantRange
	{
		std::string Name;
		uint32_t Flags;
		ShaderDataType Type;
		uint32_t Offset;
	};

	struct PipelineSpecification
	{
		std::string DebugName;
		PipelineUsage Usage = PipelineUsage::None;

		BufferLayout* pBufferLayout;
		Ref<RenderPass> pTargetRenderPass = nullptr;
		uint32_t MaxRayDepth = 0;
		uint32_t Flags = 0;

		DepthOp DepthOperator = DepthOp::LessOrEqual;

		std::unordered_map<uint32_t, std::string> Shaders;
		std::vector<DescriptorSetLayout> SetLayouts;
		std::vector<PushConstantRange> PushConstants;
	};

	class Pipeline : public RefCount
	{
	public:
		virtual ~Pipeline() = default;

		virtual const PipelineSpecification& GetSpecifications() const = 0;

		virtual Ref<Shader> GetShader() = 0;
		virtual void SetRenderPass(Ref<RenderPass> renderPass) = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void Invalidate() = 0;
		virtual bool IsValid() const = 0;

		static Ref<Pipeline> Create(PipelineSpecification* specs);
	};
}
