#pragma once

#include <hzrpch.h>
#include "Material.h"
#include "../RenderUtils.h"

namespace Hazard::Rendering {

	Material::Material()
	{
	}
	Material::~Material()
	{

	}
	void Material::Bind()
	{
		m_Pipeline->GetShader()->Bind();
		m_Pipeline->GetShader()->SetUniformInt("albedoMap", 0);

		for (int i = 0; i < m_Textures.size(); i++) {
			m_Textures[i]->Bind(i);
		}

		m_Pipeline->GetShader()->SetUniformColor("u_color", m_MaterialBuffer.color);
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
		pipelineInfo.shaderPipelineName = "PBRPipeline";
		pipelineInfo.rasterizer = &rasterizer;
		pipelineInfo.viewport = &viewport;
		pipelineInfo.stageCount = 2;
		pipelineInfo.stages = stages;

		Ref<GraphicsPipeline> pipeline = RenderUtils::Create<GraphicsPipeline>(pipelineInfo);
		Ref<Material> material = Ref<Material>::Create();
		material->SetPipeline(pipeline);

		return material;
	}
}
