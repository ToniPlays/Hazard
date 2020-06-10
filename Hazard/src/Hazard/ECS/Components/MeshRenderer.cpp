#pragma once

#include <hzrpch.h>
#include "Hazard/ECS/ECS.h"
#include "Hazard/Modules/Renderer/GlobalRenderer.h"
#include "Hazard/ECS/Camera.h"
#include "Hazard/Utils/Maths/Vector/Matrix4.h"

namespace Hazard {

	MeshRenderer::MeshRenderer(Mesh* mesh, Shader* shader, GameObject* gameObject) : Component(gameObject, "Mesh Renderer")
	{
		this->mesh = mesh;
		this->shader = shader;

		vertexArray = std::unique_ptr<VertexArray>(RendererAPI::VertexArray());

		VertexBuffer* vertexBuffer = RendererAPI::VertexBuffer(ShaderDataType::Float3, "position");
		vertexBuffer->SetData(mesh->GetVertices(), mesh->GetVerticesLength());

		VertexBuffer* textureCoords = RendererAPI::VertexBuffer(ShaderDataType::Float3, "position");
		textureCoords->SetData(mesh->GetTextureCoords(), mesh->GetVerticesLength());


		VertexBuffer* normalBuffer = RendererAPI::VertexBuffer(ShaderDataType::Float3, "normals");
		normalBuffer->SetData(mesh->GetNormals(), mesh->GetVerticesLength());

		vertexArray->SetLayout({ vertexBuffer, textureCoords, normalBuffer });
		IndexBuffer* indexBuffer = RendererAPI::IndexBuffer();

		indexBuffer->SetData(mesh->GetIndices(), mesh->GetIndicesLength());
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
		vertexArray->Bind();
		shader->Bind();
		texture->Bind();

		shader->SetUniform("projection", GlobalRenderer::Instance->Get3DProjection());
		shader->SetUniform("view", Matrix4::GetModelMatrix(Camera::GetTransform()));
		shader->SetUniform("transform", Matrix4::GetModelMatrix(gameObject->transform));
		shader->SetUniform("lightPos", { 0, 0, -20 });
		shader->SetUniform("lightColor", Vector3<float>(1, 1, 1));
		shader->SetUniform("test", (float)((Math::Sin(Time::time) + 1)) / 2);

		GlobalRenderer::Draw(mesh);

		gameObject->transform.rotation.x += Time::deltaTime * GlobalRenderer::speed, 0.0f, 360.0f;
		gameObject->transform.rotation.x = Math::ToRange(gameObject->transform.rotation.x, 0.0f, 360.0f);
	}
}