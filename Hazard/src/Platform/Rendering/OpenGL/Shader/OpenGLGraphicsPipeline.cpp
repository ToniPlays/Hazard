#pragma once

#include <hzrpch.h>
#include "OpenGLGraphicsPipeline.h"
#include "OpenGLShader.h"
#include "../OpenGLUtils.h"
#include <glad/glad.h>

namespace Hazard::Rendering::OpenGL 
{
	OpenGLGraphicsPipeline::OpenGLGraphicsPipeline(GraphicsPipelineCreateInfo info)
	{
		m_Info.rawDepthFunc = OpenGLUtils::DepthFuncToGL(info.rasterizer->depthFunc);
		m_Info.rawCullFace = OpenGLUtils::CullFaceToGL(info.rasterizer->cullFace);
		m_Info.viewport = *info.viewport;

		m_Info.pipelineShader = Ref<OpenGLShader>::Create(info.stageCount, info.stages);
	}
	OpenGLGraphicsPipeline::~OpenGLGraphicsPipeline()
	{

	}
	void OpenGLGraphicsPipeline::Bind()
	{
		glDepthFunc(m_Info.rawDepthFunc);
		glCullFace(m_Info.rawCullFace);
		
		glm::vec2 o = m_Info.viewport.offset;
		glm::vec2 s = m_Info.viewport.size;

		glViewport(o.x, o.y, s.x, s.y);

		m_Info.pipelineShader->Bind();
		
	}
}