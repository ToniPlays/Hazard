#pragma once
#include <hzrpch.h>
#include "OpenGLPipeline.h"

namespace Hazard::Rendering::OpenGL
{
	OpenGLPipeline::OpenGLPipeline(const PipelineSpecification& specs) : m_Specs(specs)
	{
		m_Shader = Shader::Create(specs.ShaderPath);


	}
	OpenGLPipeline::~OpenGLPipeline()
	{

	}
	void OpenGLPipeline::Invalidate()
	{

	}
	void OpenGLPipeline::Bind()
	{
		m_Shader->Bind();
	}
}