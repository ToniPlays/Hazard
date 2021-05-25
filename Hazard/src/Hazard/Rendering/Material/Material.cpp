#pragma once

#include <hzrpch.h>
#include "Material.h"
#include "../RenderUtils.h"

namespace Hazard::Rendering {

	Material::Material(GraphicsPipeline* pipeline)
	{
		m_Pipeline = pipeline;
	}
	Material::~Material()
	{

	}
	Ref<Material> Material::Create()
	{
		PipelineShaderStage stages[2];

		stages[0].shaderFileName = "pbr_vert.glsl";
		stages[0].stage = ShaderType::VertexShader;
		stages[0].fileType = ShaderFileType::Source;

		stages[1].shaderFileName = "pbr_frag.glsl";
		stages[1].stage = ShaderType::FragmentShader;
		stages[1].fileType = ShaderFileType::Source;

		PipelineRasterizer rasterizer;
		rasterizer.cullFace = CullFace::BackFace;
		rasterizer.depthFunc = DepthFunc::Less;

		PipelineViewport viewport;
		viewport.size = { 1920, 1080 };

		GraphicsPipelineCreateInfo pipelineInfo;
		pipelineInfo.rasterizer = &rasterizer;
		pipelineInfo.viewport = &viewport;
		pipelineInfo.stageCount = 2;
		pipelineInfo.stages = stages;

		GraphicsPipeline* pipeline = RenderUtils::CreateRaw<GraphicsPipeline>(pipelineInfo);

		return Ref<Material>::Create(pipeline);
	}
}
