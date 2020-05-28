#pragma once

#include <hzrpch.h>
#include "Hazard/ECS/ECS.h"
#include "Hazard/Modules/Rendering/Renderer.h"
#include "Hazard/ECS/Camera.h"

namespace Hazard {
	
	MeshRenderer::MeshRenderer(Mesh* mesh, Shader* shader, GameObject* gameObject) : Component(gameObject, "Mesh Renderer")
	{
		this->mesh = mesh;
		this->shader = shader;

		vertexArray = std::unique_ptr<VertexArray>(RendererAPI::VertexArray());

		VertexBuffer* vertexBuffer = RendererAPI::VertexBuffer(ShaderDataType::Float3, "position");
		VertexBuffer* colorBuffer = RendererAPI::VertexBuffer(ShaderDataType::Float2, "textureCoords");

		vertexBuffer->SetData(mesh->GetVertices());
		colorBuffer->SetData(mesh->GetTextureCoords());

		vertexArray->SetLayout({ vertexBuffer, colorBuffer });

		IndexBuffer* indexBuffer = RendererAPI::IndexBuffer();

		indexBuffer->SetData(mesh->GetIndices(), sizeof(mesh->GetIndices()) * sizeof(int));
		vertexArray->SetIndexBuffer(indexBuffer);

		texture = RendererAPI::Texture2D("res/textures/checker.png");
		texture->Bind();

		vertexArray->Bind();
	}

	MeshRenderer::~MeshRenderer()
	{
		delete mesh;
		delete shader;
	}

	void MeshRenderer::OnRender()
	{
		shader->Bind();
		texture->Bind();

		Matrix4 view = Renderer::GetViewMatrix();
		view.Mul(Matrix4::TransformMatrix(Camera::instance->transform));

		shader->SetUniform("projection", view);
		shader->SetUniform("transform", Matrix4::TransformMatrix(gameObject->transform));
		shader->SetUniform("test", 1.0f);
		shader->SetUniform("gradient", 1);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

}