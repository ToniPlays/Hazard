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
		bool IsValid() const override { return m_Shader.As<OpenGLShader>()->GetProgramID(); }
		bool IsCompatibleWith(Ref<Shader> shader) const;

		void SetLayout(const BufferLayout& layout) { m_Layout = layout; };

		//OpenGL specific;
		uint32_t GetDrawType() { return m_DrawType; }
		uint32_t GetPolygonMode() { return m_PolygonMode; }
		bool DepthMaskEnable() { return m_Specs.DepthWrite; }

	private:
		PipelineSpecification m_Specs;
		BufferLayout m_Layout;
		Ref<Shader> m_Shader;
		uint32_t m_DrawType, m_PolygonMode;
	};
}
#endif
