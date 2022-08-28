#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL

#include "Backend/Core/Pipeline/Pipeline.h"
#include "OpenGLShader.h"

namespace HazardRenderer::OpenGL
{
	class OpenGLPipeline : public Pipeline 
	{
	public:
		OpenGLPipeline(PipelineSpecification* specs);
		virtual ~OpenGLPipeline();

		PipelineSpecification GetSpecifications() { return m_Specs; }
		const PipelineSpecification GetSpecifications() const { return m_Specs; }
		Ref<Shader> GetShader() { return m_Shader; }
		void SetRenderPass(Ref<RenderPass> renderPass) override { m_Specs.pTargetRenderPass = renderPass.Raw(); }

		void Invalidate() override;
		//void Bind(Ref<RenderCommandBuffer> commandBuffer);
		//void Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count);
		//void DrawInstanced(Ref<RenderCommandBuffer> commandBuffer, uint32_t count, uint32_t instanceCount);
		//void DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count);

	private:
		PipelineSpecification m_Specs;
		Ref<Shader> m_Shader;
		uint32_t m_DrawType, m_PolygonMode;
	};
}
#endif
