
#include "OpenGLPipeline.h"
#ifdef HZR_INCLUDE_OPENGL
#include "../OpenGLFramebuffer.h"
#include "../OpenGLUtils.h"
#include <glad/glad.h>

namespace HazardRenderer::OpenGL
{
	OpenGLPipeline::OpenGLPipeline(PipelineSpecification* specs) : m_Specs(*specs)
	{
		HZR_ASSERT(specs->pBufferLayout, "Cannot use pipeline without input layout");
		m_Shader = Shader::Create(specs->ShaderPath);
		Invalidate();
	}
	OpenGLPipeline::~OpenGLPipeline()
	{
	}
	void OpenGLPipeline::Invalidate()
	{
		m_DrawType = OpenGLUtils::DrawTypeToGLType(m_Specs.DrawType);
		m_PolygonMode = OpenGLUtils::DrawTypeToGLPolygon(m_Specs.DrawType);
	}
	void OpenGLPipeline::Bind(Ref<RenderCommandBuffer> commandBuffer)
	{
		m_Shader->Bind(commandBuffer);
		glPolygonMode(GL_FRONT_AND_BACK, m_PolygonMode);
		glLineWidth(m_Specs.LineWidth);

		m_Specs.DepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		switch (m_Specs.CullMode)
		{
		case CullMode::None: glDisable(GL_CULL_FACE); break;
		case CullMode::FrontFace: 
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT); 
			break;
		case CullMode::BackFace: 
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK); 
			break;
		}

	}
	void OpenGLPipeline::Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
	{
		glDrawElements(m_DrawType, count, GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLPipeline::DrawInstanced(Ref<RenderCommandBuffer> commandBuffer, uint32_t count, uint32_t instanceCount)
	{
		glDrawElementsInstanced(m_DrawType, count, GL_UNSIGNED_INT, nullptr, instanceCount);
	}
	void OpenGLPipeline::DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
	{
		glDrawArrays(m_DrawType, 0, count);
	}
}
#endif
