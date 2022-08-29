#pragma once

#include "BufferLayout.h"
#include "Buffers.h"
#include "Shader.h"
#include <glad/glad.h>

namespace HazardRenderer
{
	enum class PipelineUsage { None = 0, GraphicsBit, ComputeBit };
	enum class DrawType { None = 0, Fill, Line, Point };
	enum class CullMode { None = 0, FrontFace, BackFace };
	enum class DepthOp {
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

	class RenderPass;

	struct PipelineSpecification
	{
		std::string DebugName;
		std::string ShaderPath;
		BufferLayout* pBufferLayout;
		PipelineUsage Usage = PipelineUsage::None;
		DrawType DrawType = DrawType::Fill;
		CullMode CullMode = CullMode::BackFace;
		float LineWidth = 1.0f;
		bool IsShared = true;
		bool DepthTest = true;
		bool DepthWrite = true;
		DepthOp DepthOperator = DepthOp::LessOrEqual;
		Ref<RenderPass> pTargetRenderPass = nullptr;
	};

	class Pipeline : public HZR_RESOURCE_HANDLER {
	public:

		virtual ~Pipeline() = default;
		virtual PipelineSpecification GetSpecifications() = 0;
		virtual const PipelineSpecification GetSpecifications() const = 0;

		virtual Ref<Shader> GetShader() = 0;
		virtual void SetRenderPass(Ref<RenderPass> renderPass) = 0;

		virtual void Invalidate() = 0;
		virtual bool IsValid() const = 0;

		static Ref<Pipeline> Create(PipelineSpecification* specs);
	};
}