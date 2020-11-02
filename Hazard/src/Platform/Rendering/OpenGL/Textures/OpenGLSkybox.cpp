#pragma once

#include <hzrpch.h>
#include "OpenGLSkybox.h"
#include "Hazard/Modules/Renderer/RenderEngine.h"

namespace Hazard {

	OpenGLSkybox::OpenGLSkybox()
	{

		float size = 1;
		float vertices[24] = {
			-1.0f * size, -1.0f * size, -1.0f * size,
			 1.0f * size, -1.0f * size, -1.0f * size,
			 1.0f * size,  1.0f * size, -1.0f * size,
			-1.0f * size,  1.0f * size, -1.0f * size,
			-1.0f * size, -1.0f * size,  1.0f * size,
			 1.0f * size, -1.0f * size,  1.0f * size,
			 1.0f * size,  1.0f * size,  1.0f * size,
			-1.0f * size,  1.0f * size,  1.0f * size
		};

		std::cout << sizeof(vertices) << std::endl;
		vertexArray = RendererAPI::Create<VertexArray>();
		VertexBuffer* buffer = RendererAPI::VertexBuffer(sizeof(vertices));

		buffer->SetLayout({
			{ ShaderDataType::Float3, "position" }
		});


		uint32_t indices[36] = {
			0, 1, 3, 3, 1, 2,
			1, 5, 2, 2, 5, 6,
			5, 4, 6, 6, 4, 7,
			4, 0, 7, 7, 0, 3,
			3, 2, 7, 7, 2, 6,
			4, 5, 0, 0, 5, 1
		};

		buffer->SetData(vertices, sizeof(vertices));
		vertexArray->AddBuffer(buffer);

		IndexBuffer* indexBuffer = RendererAPI::Create<IndexBuffer>();
		indexBuffer->SetData(indices, 36);

		vertexArray->SetIndexBuffer(indexBuffer);
	}
	OpenGLSkybox::~OpenGLSkybox()
	{
		delete skyboxShader;
		delete vertexArray;
	}
	void OpenGLSkybox::Render()
	{
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_FRONT);
		vertexArray->EnableAll();
		texture->Bind();

		RenderEngine::Draw(vertexArray, vertexArray->GetIndexBuffer()->GetCount());

		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);
	}
	void OpenGLSkybox::SetCubemap(CubemapTexture* texture)
	{
		this->texture = texture;
	}
}