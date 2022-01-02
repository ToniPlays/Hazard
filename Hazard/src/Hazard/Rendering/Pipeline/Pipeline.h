#pragma once

#include "Buffers.h"
#include "Shader.h"
#include "RenderPass.h"
#include "Hazard/Rendering/RenderCommandBuffer.h"

namespace Hazard::Rendering
{
	enum class PipelineUsage { None = 0, GraphicsBit, ComputeBit };
	enum class DrawType { None = 0, Fill, Line, Point };

	struct PipelineSpecification
	{
		std::string ShaderPath;
		PipelineUsage Usage = PipelineUsage::None;
		DrawType DrawType;
		float LineWidth = 1.0f;
		bool Culling = true;
		bool IsBackground = false;
		Ref<RenderPass> TargetRenderPass = nullptr;
	};

	class Pipeline : public RefCount {
	public:
		virtual ~Pipeline() = default;

		virtual PipelineSpecification GetSpecifications() = 0;
		virtual const PipelineSpecification GetSpecifications() const = 0;

		virtual Ref<Shader> GetShader() = 0;

		virtual void Invalidate() = 0;
		virtual void Bind(Ref<RenderCommandBuffer> commandBuffer) = 0;
		virtual void Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) = 0;
		virtual void DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) = 0;

		static Ref<Pipeline> Create(PipelineSpecification* specs);
	};
}