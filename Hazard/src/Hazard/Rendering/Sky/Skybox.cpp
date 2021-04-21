#pragma once

#include <hzrpch.h>
#include "Skybox.h"
#include "../RenderCommand.h"

namespace Hazard::Rendering {

	Skybox::Skybox()
	{
		m_VAO = RenderUtils::Create<VertexArray>();

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
		VertexBuffer* buffer = RenderUtils::Create<VertexBuffer>((uint32_t)(24 * sizeof(float)));

		buffer->SetData(skyboxVertices, 24 * sizeof(float));

		buffer->SetLayout({ {ShaderDataType::Float3, "skybox_pos"} });
		m_VAO->AddBuffer(buffer);

		uint32_t indices[] = {
			0, 1, 3, 3, 1, 2,
			1, 5, 2, 2, 5, 6,
			5, 4, 6, 6, 4, 7,
			4, 0, 7, 7, 0, 3,
			3, 2, 7, 7, 2, 6,
			4, 5, 0, 0, 5, 1
		};

		IndexBuffer* iBuffer = RenderUtils::Create<IndexBuffer>();
		iBuffer->SetData(indices, 36);
		m_VAO->SetIndexBuffer(iBuffer);

		m_SkyboxShader = RenderUtils::Create<Shader>("res/shaders/skybox.glsl");
		m_SkyboxShader->SetUniformInt("SkyboxCubemap", 0);
		m_SkyboxShader->Bind();
		m_SkyboxShader->Unbind();
	}
	Skybox::~Skybox()
	{
		delete m_VAO;
	}
	void Skybox::Render(glm::mat4 transform)
	{
		RenderContextCommand::SetDepthTest(DepthTest::LEqual);
		m_SkyboxShader->Bind();
		m_SkyboxShader->SetUniformMat4("viewProjection", transform);
		m_Texture->Bind(0);
		RenderCommand::DrawIndexed(m_VAO, 36);
		RenderContextCommand::SetDepthTest(DepthTest::Less);
	}
}