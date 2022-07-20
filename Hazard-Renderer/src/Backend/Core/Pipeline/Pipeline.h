#pragma once

#include "Buffers.h"
#include "RenderPass.h"
#include "Shader.h"

namespace HazardRenderer
{
	enum class PipelineUsage { None = 0, GraphicsBit, ComputeBit };
	enum class DrawType { None = 0, Fill, Line, Point };
	enum class CullMode { None = 0, FrontFace, BackFace };

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
		RenderPass* pTargetRenderPass = nullptr;
	};

	class Pipeline : public RefCount {
	public:
		virtual ~Pipeline() = default;

		virtual PipelineSpecification GetSpecifications() = 0;
		virtual const PipelineSpecification GetSpecifications() const = 0;

		virtual Ref<Shader> GetShader() = 0;
		virtual void SetRenderPass(Ref<RenderPass> renderPass) = 0;

		virtual void Invalidate() = 0;
		virtual void Bind(Ref<RenderCommandBuffer> commandBuffer) = 0;
		virtual void Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) = 0;
		virtual void DrawInstanced(Ref<RenderCommandBuffer> commandBuffer, uint32_t count, uint32_t instanceCount) = 0;
		virtual void DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) = 0;

		static Ref<Pipeline> Create(PipelineSpecification* specs);
	};
}