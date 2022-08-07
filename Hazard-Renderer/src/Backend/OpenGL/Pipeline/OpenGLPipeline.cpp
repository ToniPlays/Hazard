
#include "OpenGLPipeline.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/Renderer.h"
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
		Ref<OpenGLPipeline> instance = this;
		Renderer::Submit([instance, cmdBuffer = commandBuffer]() mutable {
			HZR_PROFILE_FUNCTION("OpenGLPipeline::Bind(Ref) RT");
			instance->m_Shader->Bind_RT(cmdBuffer);
			glPolygonMode(GL_FRONT_AND_BACK, instance->m_PolygonMode);
			if (instance->m_Specs.LineWidth != 1.0f)
				glLineWidth(instance->m_Specs.LineWidth);

			instance->m_Specs.DepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			switch (instance->m_Specs.CullMode)
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
			});
	}
	void OpenGLPipeline::Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
	{
		Renderer::Submit([drawType = m_DrawType, count]() mutable {
			HZR_PROFILE_FUNCTION("OpenGLPipeline::Draw(Ref, uint32_t) RT");
			glDrawElements(drawType, count, GL_UNSIGNED_INT, nullptr);
			});
	}
	void OpenGLPipeline::DrawInstanced(Ref<RenderCommandBuffer> commandBuffer, uint32_t count, uint32_t instanceCount)
	{
		Renderer::Submit([drawType = m_DrawType, count, instanceCount]() mutable {
			HZR_PROFILE_FUNCTION("OpenGLPipeline::DrawInstanced(Ref, uint32_t, uint32_t) RT");
			glDrawElementsInstanced(drawType, count, GL_UNSIGNED_INT, nullptr, instanceCount);
			});
	}
	void OpenGLPipeline::DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
	{
		Renderer::Submit([drawType = m_DrawType, count]() mutable {
			HZR_PROFILE_FUNCTION("OpenGLPipeline::DrawArrays(Ref, uint32_t) RT");
			glDrawArrays(drawType, 0, count);
			});
	}
}
#endif
