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

		std::vector<ShaderStage> stages(2);

		stages[0] = { ShaderType::VertexShader,		"skybox_vert.glsl" };
		stages[1] = { ShaderType::FragmentShader,	"skybox_frag.glsl" };

		ShaderCreateInfo shaderInfo;
		shaderInfo.shaderName = "Skybox";
		shaderInfo.stages = stages;

		m_SkyboxShader = RenderUtils::Create<Shader>(shaderInfo);
		m_SkyboxShader->Bind();
		m_SkyboxShader->SetUniformInt("SkyboxCubemap", 0);
		m_SkyboxShader->Unbind();
	}
	Skybox::~Skybox()
	{

	}
	void Skybox::Render()
	{
		if (!m_Texture) return;

		RenderContextCommand::SetDepthTest(DepthFunc::LessOrEqual);
		m_SkyboxShader->Bind();
		m_Texture->Bind(0);
		RenderCommand::DrawIndexed(m_VAO, 36);
		RenderContextCommand::SetDepthTest(DepthFunc::Less);
	}
}