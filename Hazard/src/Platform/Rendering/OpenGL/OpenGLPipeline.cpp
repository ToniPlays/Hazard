#pragma once
#include <hzrpch.h>
#include "OpenGLPipeline.h"
#include "OpenGLFrameBuffer.h"
#include "OpenGLUtils.h"
#include <glad/glad.h>

namespace Hazard::Rendering::OpenGL
{
	OpenGLPipeline::OpenGLPipeline(PipelineSpecification* specs) : m_Specs(*specs)
	{
		HZR_PROFILE_FUNCTION();

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
		m_Shader->Bind();
		glPolygonMode(GL_FRONT_AND_BACK, m_PolygonMode);
		glLineWidth(m_Specs.LineWidth);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (m_Specs.Culling) glEnable(GL_CULL_FACE); 
		else glDisable(GL_CULL_FACE);

	}
	void OpenGLPipeline::Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
	{
		glDrawElements(m_DrawType, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLPipeline::DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
	{
		glDrawArrays(m_DrawType, 0, count);
	}
}