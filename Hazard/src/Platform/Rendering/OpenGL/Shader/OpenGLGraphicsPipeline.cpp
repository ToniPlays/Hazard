#pragma once

#include <hzrpch.h>
#include "OpenGLGraphicsPipeline.h"
#include "../OpenGLUtils.h"
#include <glad/glad.h>

namespace Hazard::Rendering::OpenGL 
{
	OpenGLGraphicsPipeline::OpenGLGraphicsPipeline(GraphicsPipelineCreateInfo info)
	{
		m_Info.rawDepthFunc = OpenGLUtils::DepthFuncToGL(info.rasterizer->depthFunc);
		m_Info.viewport = *info.viewport;
	}
	OpenGLGraphicsPipeline::~OpenGLGraphicsPipeline()
	{

	}
	void OpenGLGraphicsPipeline::Bind()
	{
		PipelineViewport& v = m_Info.viewport;

		glViewport(v.offset.x, v.offset.y, v.size.x, v.size.y);
		glDepthFunc(m_Info.rawDepthFunc);
	}
}