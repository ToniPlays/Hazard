#pragma once

#include <hzrpch.h>
#include "OpenGLGraphicsPipeline.h"
#include "OpenGLShader.h"
#include "Hazard/Rendering/RenderUtils.h"
#include "../OpenGLUtils.h"
#include <glad/glad.h>

namespace Hazard::Rendering::OpenGL 
{
	OpenGLGraphicsPipeline::OpenGLGraphicsPipeline(GraphicsPipelineCreateInfo info)
	{
		m_Info.rawDepthFunc = OpenGLUtils::DepthFuncToGL(info.rasterizer->depthFunc);
		m_Info.rawCullFace = OpenGLUtils::CullFaceToGL(info.rasterizer->cullFace);
		m_Info.viewport = *info.viewport;

		m_Info.pipelineShader = RenderUtils::Create<Shader>(info.shaderPipelineName.c_str(), info.stageCount, info.stages);
	}
	OpenGLGraphicsPipeline::~OpenGLGraphicsPipeline()
	{

	}
	void OpenGLGraphicsPipeline::Bind()
	{
		glDepthFunc(m_Info.rawDepthFunc);
		glCullFace(m_Info.rawCullFace);

		m_Info.pipelineShader->Bind();
	}
}