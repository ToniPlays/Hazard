#pragma once

#include <hzrpch.h>
#include "Skybox.h"
#include "../RenderCommand.h"

namespace Hazard::Rendering {

	Skybox::Skybox()
	{
		float skyboxVertices[] = {
			// positions
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f
		};
		uint32_t indices[] = {
			0, 1, 3, 3, 1, 2,
			1, 5, 2, 2, 5, 6,
			5, 4, 6, 6, 4, 7,
			4, 0, 7, 7, 0, 3,
			3, 2, 7, 7, 2, 6,
			4, 5, 0, 0, 5, 1
		};

		BufferLayout layout = { {ShaderDataType::Float3, "skybox_pos"} };

		VertexBufferCreateInfo bufferInfo;
		bufferInfo.dataStream = DataStream::StaticDraw;
		bufferInfo.layout = &layout;
		bufferInfo.size = 24 * sizeof(float);
		bufferInfo.data = &skyboxVertices;

		IndexBufferCreateInfo indexBufferInfo;
		indexBufferInfo.size = 36;
		indexBufferInfo.data = indices;

		VertexArrayCreateInfo createInfo;
		createInfo.bufferInfo = &bufferInfo;
		createInfo.indexBufferInfo = &indexBufferInfo;

		m_VAO = RenderUtils::CreateRaw<VertexArray>(createInfo);

		PipelineRasterizer rasterizer = {};
		rasterizer.depthFunc = DepthFunc::LessOrEqual;
		rasterizer.cullFace = CullFace::BackFace;

		PipelineViewport viewport = {};
		viewport.size = { 1920, 1080 };

		PipelineShaderStage stages[2];
		stages[0].shaderFileName = "skybox_vert.glsl";
		stages[0].fileType = ShaderFileType::Source;
		stages[0].stage = ShaderType::VertexShader;

		stages[1].shaderFileName = "skybox_frag.glsl";
		stages[1].fileType = ShaderFileType::Source;
		stages[1].stage = ShaderType::FragmentShader;

		GraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.inputAssembly = NULL;
		pipelineInfo.viewport = &viewport;
		pipelineInfo.rasterizer = &rasterizer;
		pipelineInfo.stageCount = 2;
		pipelineInfo.stages = stages;

		m_Pipeline = RenderUtils::CreateRaw<GraphicsPipeline>(pipelineInfo);
	}
	Skybox::~Skybox()
	{

	}
	void Skybox::Render()
	{
		if (!m_Texture) return;

		m_Pipeline->Bind();
		m_Texture->Bind(0);

		RenderCommand::DrawIndexed(m_VAO, 36);
	}
}