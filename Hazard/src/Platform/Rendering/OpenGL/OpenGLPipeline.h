#pragma once

#include "Hazard/Rendering/Pipeline/Pipeline.h"
#include "OpenGLShader.h"

namespace Hazard::Rendering::OpenGL 
{
	class OpenGLPipeline : public Pipeline {
	public:
		OpenGLPipeline(const PipelineSpecification& specs);
		virtual ~OpenGLPipeline();

		virtual PipelineSpecification GetSpecifications() { return m_Specs; }
		virtual const PipelineSpecification GetSpecifications() const { return m_Specs; }
		virtual Ref<Shader> GetShader() { return m_Shader; }
		virtual Ref<VertexBuffer> GetBuffer() override { return m_Buffer; }

		virtual void Invalidate() override;
		virtual void Bind(Ref<RenderCommandBuffer> commandBuffer) override;
		virtual void Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count) override;

	private:
		PipelineSpecification m_Specs;
		Ref<Shader> m_Shader;
		Ref<VertexBuffer> m_Buffer;
		Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_ID;
		uint32_t m_DrawType;
	};
}