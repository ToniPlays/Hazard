#pragma once

#include <hzrpch.h>
#include "Material.h"
#include "../RenderUtils.h"

namespace Hazard::Rendering {

	Material::Material()
	{
		m_Textures = std::vector<Ref<Texture2D>>(2);
		Ref<Texture2D> white = Vault::Get<Texture2D>("White");
		for (int i = 0; i < m_Textures.size(); i++) {
			m_Textures[i] = white;
		}

		m_Textures[1] = Ref<Texture2D>(Vault::Get<Texture2D>("DefaultNormalMap"));
	}
	Material::~Material()
	{

	}
	void Material::Bind()
	{
		Shader* shader = m_Pipeline->GetShader().Raw();
		shader->Bind();
		shader->SetUniformInt("albedoMap", 1);
		shader->SetUniformInt("normalMap", 2);

		for (int i = 0; i < m_Textures.size(); i++) {
			m_Textures[i]->Bind(i + 1);
		}

		shader->SetUniformColor("u_color", m_MaterialBuffer.color);
		shader->SetUniformFloat("u_metallic", m_MaterialBuffer.metallic);
		shader->SetUniformFloat("u_smoothness", m_MaterialBuffer.smoothness);
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
