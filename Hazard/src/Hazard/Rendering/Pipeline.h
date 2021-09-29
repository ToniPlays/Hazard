#pragma once

#include "Buffers/Buffers.h"
#include "Shader.h"

namespace Hazard::Rendering
{
	enum PipelineUsage { NoUsage = 0, GraphicsBit, ComputeBit };

	struct PipelineSpecification
	{
		std::string ShaderPath;
		PipelineUsage Usage = PipelineUsage::NoUsage;
		VertexBufferCreateInfo* pVertexBuffer = nullptr;
		IndexBufferCreateInfo* pIndexBuffer = nullptr;
	};


	class Pipeline : public RefCount {
	public:
		virtual ~Pipeline() = default;

		virtual PipelineSpecification GetSpecifications() = 0;
		virtual const PipelineSpecification GetSpecifications() const = 0;

		virtual Ref<VertexBuffer> GetBuffer() = 0;
		virtual Ref<Shader> GetShader() = 0;

		virtual void Invalidate() = 0;
		virtual void Bind() = 0;
		virtual void Draw(uint32_t count) = 0;

		static Ref<Pipeline> Create(const PipelineSpecification& specs);
	};
}