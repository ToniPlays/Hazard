#pragma once

#include <hzrpch.h>
#include "OpenGLVignette.h"
#include "Hazard/Rendering/RenderUtils.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "Hazard/Rendering/PostProcessing/PostProcessingStack.h"

namespace Hazard::Rendering::OpenGL {

	OpenGLVignette::OpenGLVignette(VignetteEffectCreateInfo* info)
	{
		m_Inner = info->inner;
		m_Outer = info->outer;
		m_Intensity = info->intensity;
		use = info->use;

		FrameBufferCreateInfo frameBufferInfo;
		frameBufferInfo.name = "Vignette";
		frameBufferInfo.width = 1920;
		frameBufferInfo.height = 1080;
		frameBufferInfo.attachments = { FrameBufferTextureFormat::RGBA8 };

		m_Target = new OpenGLFrameBuffer(frameBufferInfo);

		PipelineShaderStage stages[2];

		stages[0].shaderFileName = "res/shaders/compiled/passtrough_vert.glsl";
		stages[0].stage = ShaderType::VertexShader;
		stages[0].fileType = ShaderFileType::Source;

		stages[1].shaderFileName = "res/shaders/compiled/vignette_frag.glsl";
		stages[1].stage = ShaderType::FragmentShader;
		stages[1].fileType = ShaderFileType::Source;

		PipelineRasterizer rasterizer = {};
		rasterizer.cullFace = CullFace::BackFace;
		rasterizer.depthFunc = DepthFunc::Less;

		PipelineViewport viewport;
		viewport.size = { 1280, 720 };

		GraphicsPipelineCreateInfo pipelineInfo;
		pipelineInfo.viewport = &viewport;
		pipelineInfo.rasterizer = &rasterizer;
		pipelineInfo.stageCount = 2;
		pipelineInfo.stages = stages;

		m_Pipeline = RenderUtils::CreateRaw<GraphicsPipeline>(pipelineInfo);

		m_Pipeline->GetShader()->Bind();
		m_Pipeline->GetShader()->SetUniformInt("sampleTexture", 0);
	}

	OpenGLVignette::~OpenGLVignette()
	{

	}

	FrameBuffer* OpenGLVignette::Process(FrameBuffer* source, VertexArray* vao)
	{
		glBindTexture(GL_TEXTURE_2D, source->GetColorID());
		m_Target->Bind();
		RenderContextCommand::ClearFrame({ 0, 0, 0, 1.0 });
		m_Pipeline->Bind();

		RenderCommand::DrawIndexed(vao, vao->GetIndexBuffer()->GetInfo().count);

		return m_Target;
	}
	void OpenGLVignette::UpdateData(PostProcessData& data)
	{
		data.v_Outer = m_Outer;
		data.v_Inner = m_Inner;
		data.v_Intensity = m_Intensity;
	}
}
