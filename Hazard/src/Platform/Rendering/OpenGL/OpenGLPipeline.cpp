#pragma once
#include <hzrpch.h>
#include "OpenGLPipeline.h"

namespace Hazard::Rendering::OpenGL
{
	OpenGLPipeline::OpenGLPipeline(const PipelineSpecification& specs)
	{
		m_Shader = new OpenGLShader(specs.ShaderPath);
	}
	OpenGLPipeline::~OpenGLPipeline()
	{

	}
	void OpenGLPipeline::Invalidate()
	{

	}
	void OpenGLPipeline::Bind()
	{

	}
}