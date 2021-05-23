#pragma once
#include <hzrpch.h>
#include "OpenGLBloom.h"
#include "OpenGLGaussianBlur.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "Hazard/Rendering/PostProcessing/PostProcessingStack.h"

namespace Hazard::Rendering::OpenGL {

	OpenGLBloom::OpenGLBloom(BloomCreateInfo* info)
	{
		use = info->use;
		threshold = info->threshold;
		intensity = info->intensity;

		FrameBufferCreateInfo frameBufferInfo;
		frameBufferInfo.name = "GaussianBlur";
		frameBufferInfo.width = 1920;
		frameBufferInfo.height = 1080;
		frameBufferInfo.attachments = { FrameBufferTextureFormat::RGBA8 };

		m_Target = new OpenGLFrameBuffer(frameBufferInfo);

		PipelineShaderStage stages[2];

		stages[0].shaderFileName = "res/shaders/compiled/passtrough_vert.glsl";
		stages[0].stage = ShaderType::VertexShader;
		stages[0].fileType = ShaderFileType::Source;

		stages[1].shaderFileName = "res/shaders/compiled/bloom_frag.glsl";
		stages[1].stage = ShaderType::FragmentShader;
		stages[1].fileType = ShaderFileType::Source;

		PipelineRasterizer rasterizer = {};
		rasterizer.cullFace = CullFace::BackFace;
		rasterizer.depthFunc = DepthFunc::Less;

		PipelineViewport viewport;
		viewport.size = { 1280, 720 };

		GraphicsPipelineCreateInfo pipelineInfo;
		pipelineInfo.shaderPipelineName = "BloomPostProcess";
		pipelineInfo.viewport = &viewport;
		pipelineInfo.rasterizer = &rasterizer;
		pipelineInfo.stageCount = 2;
		pipelineInfo.stages = stages;

		m_Pipeline = RenderUtils::CreateRaw<GraphicsPipeline>(pipelineInfo);
		m_Pipeline->GetShader()->SetUniformInt("sampleTexture", 0);

		stages[0].shaderFileName = "res/shaders/compiled/passtrough_vert.glsl";
		stages[1].shaderFileName = "res/shaders/compiled/combine_frag.glsl";
		pipelineInfo.shaderPipelineName = "CombineShader";

		m_CombinePipeline = RenderUtils::CreateRaw<GraphicsPipeline>(pipelineInfo);
		m_CombinePipeline->GetShader()->Bind();
		m_CombinePipeline->GetShader()->SetUniformInt("sampleTextures", 1);
	}
	OpenGLBloom::~OpenGLBloom()
	{

	}
	FrameBuffer* OpenGLBloom::Process(FrameBuffer* source, VertexArray* vao)
	{
		//Scene Color
		glBindTextureUnit(0, source->GetColorID());
		m_Pipeline->GetShader()->Bind();
		m_Target->Bind();
		RenderContextCommand::ClearFrame({0, 0, 0, 1.0});
		RenderCommand::DrawIndexed(vao, vao->GetIndexBuffer()->GetInfo().count);
		
		GaussianBlurEffect* gaussian = PostProcessingStack::Get<GaussianBlurEffect>();
		FrameBuffer* result = gaussian->Process(m_Target, vao);

		glBindTextureUnit(0, source->GetColorID());
		glBindTextureUnit(1, result->GetColorID());

		m_Target->Bind();
		RenderContextCommand::ClearFrame({ 0, 0, 0, 1.0 });
		m_CombinePipeline->GetShader()->Bind();
		RenderCommand::DrawIndexed(vao, vao->GetIndexBuffer()->GetInfo().count);
		
		return m_Target;
	}
	void OpenGLBloom::UpdateData(PostProcessData& data)
	{
		data.bloomThreshold = threshold;
		data.bloomIntensity = intensity;
	}
}
