#pragma once

#include <hzrpch.h>
#include "OpenGLVignette.h"
#include "Hazard/Rendering/RenderUtils.h"
#include "Hazard/Rendering/RenderCommand.h"

namespace Hazard::Rendering::OpenGL {

	OpenGLVignette::OpenGLVignette()
	{

		FrameBufferCreateInfo info;
		info.name = "Vignette";
		info.width = 1920;
		info.height = 1080;
		info.attachments = { FrameBufferTextureFormat::RGBA8 };

		m_Target = new OpenGLFrameBuffer(info);

		PipelineShaderStage stages[2];

		stages[0].shaderFileName = "res/shaders/compiled/vignette_vert.glsl";
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

		BufferLayout layout = { {ShaderDataType::Float2, "pos"}, { ShaderDataType::Float2, "texCoords"} };

		float vertices[] = {
			-1.0, -1.0f, 0.0f, 0.0f,
			 1.0, -1.0f, 1.0f, 0.0f,
			 1.0,  1.0f, 1.0f, 1.0f,
			-1.0,  1.0f, 0.0f, 1.0f
		};
		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexBufferCreateInfo bufferInfo;
		bufferInfo.dataStream = DataStream::DynamicDraw;
		bufferInfo.layout = &layout;
		bufferInfo.data = &vertices;
		bufferInfo.size = sizeof(vertices);

		IndexBufferCreateInfo indexInfo;
		indexInfo.dataStream = DataStream::StaticDraw;
		indexInfo.size = 6;
		indexInfo.data = indices;

		VertexArrayCreateInfo arrayInfo;
		arrayInfo.bufferInfo = &bufferInfo;
		arrayInfo.indexBufferInfo = &indexInfo;

		m_VertexArray = RenderUtils::CreateRaw<VertexArray>(arrayInfo);
	}

	OpenGLVignette::~OpenGLVignette()
	{

	}

	void OpenGLVignette::Process(FrameBuffer* source, glm::vec2 res)
	{
		glBindTexture(GL_TEXTURE_2D, source->GetColorID(0));
		m_Target->Bind();
		m_Pipeline->Bind();
		m_Pipeline->GetShader()->SetUniformVec2("resolution", res);

		RenderContextCommand::ClearFrame({ 1, 1, 1, 1.0 });
		RenderCommand::DrawIndexed(m_VertexArray, m_VertexArray->GetIndexBuffer()->GetInfo().count);
	}
}
