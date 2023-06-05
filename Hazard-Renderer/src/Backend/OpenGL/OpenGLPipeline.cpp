
#include "OpenGLPipeline.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/Renderer.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLUtils.h"
#include <glad/glad.h>

namespace HazardRenderer::OpenGL
{
	OpenGLPipeline::OpenGLPipeline(PipelineSpecification* specs) : m_Specs(*specs)
	{
		HZR_PROFILE_FUNCTION();

		std::vector<ShaderStageCode> code(specs->ShaderCodeCount);
		for (uint32_t i = 0; i < specs->ShaderCodeCount; i++)
			code[i] = specs->pShaderCode[i];

		m_Shader = Shader::Create(code);
		Invalidate();
	}
	OpenGLPipeline::~OpenGLPipeline()
	{

	}
	void OpenGLPipeline::Invalidate()
	{
		HZR_PROFILE_FUNCTION();
		m_DrawType = OpenGLUtils::DrawTypeToGLType(m_Specs.DrawType);
		m_PolygonMode = OpenGLUtils::DrawTypeToGLPolygon(m_Specs.DrawType);
	}
	bool OpenGLPipeline::IsCompatibleWith(Ref<Shader> shader) const
	{
		auto& data = shader->GetShaderData().Stages;
		uint32_t stride = m_Specs.pBufferLayout->GetStride();
		if (data.find(ShaderStage::Vertex) == data.end() && stride == 0) return true;
		return data.at(ShaderStage::Vertex).Stride == stride;
	}
}
#endif
