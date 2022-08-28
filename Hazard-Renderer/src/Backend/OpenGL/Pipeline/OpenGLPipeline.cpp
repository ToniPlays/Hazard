
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
		__debugbreak();
	}
	void OpenGLPipeline::Invalidate()
	{
		m_DrawType = OpenGLUtils::DrawTypeToGLType(m_Specs.DrawType);
		m_PolygonMode = OpenGLUtils::DrawTypeToGLPolygon(m_Specs.DrawType);
	}
}
#endif
