#pragma once

#include <hzrpch.h>
#include "Hazard/ECS/ECS.h"
#include "Hazard/Modules/Rendering/Renderer.h"
#include "Hazard/ECS/Camera.h"
#include "../../Utils/Maths/Vector/Matrix4.h"

namespace Hazard {

	MeshRenderer::MeshRenderer(Mesh* mesh, Shader* shader, GameObject* gameObject) : Component(gameObject, "Mesh Renderer")
	{
		this->mesh = mesh;
		this->shader = shader;

		vertexArray = std::unique_ptr<VertexArray>(RendererAPI::VertexArray());

		VertexBuffer* vertexBuffer = RendererAPI::VertexBuffer(ShaderDataType::Float3, "position");
		vertexBuffer->SetData(mesh->GetVertices(), mesh->GetVerticesLength());


		HZR_CORE_INFO("Has normals");

		VertexBuffer* normalBuffer = RendererAPI::VertexBuffer(ShaderDataType::Float3, "normals");
		normalBuffer->SetData(mesh->GetNormals(), mesh->GetVerticesLength());
		HZR_CORE_INFO("Mesh has normals");

		vertexArray->SetLayout({ vertexBuffer, normalBuffer });
		IndexBuffer* indexBuffer = RendererAPI::IndexBuffer();

		indexBuffer->SetData(mesh->GetIndices(), mesh->GetIndicesLength());
		vertexArray->SetIndexBuffer(indexBuffer);

		//texture = RendererAPI::Texture2D("res/textures/checker.png");
		//texture->Bind();

		vertexArray->Bind();

	}

	MeshRenderer::~MeshRenderer()
	{
		delete mesh;
		delete shader;
	}

	void MeshRenderer::OnRender()
	{
		//texture->Bind();

		vertexArray->Bind();
		shader->Bind();

		shader->SetUniform("projection", Renderer::GetProjection());
		shader->SetUniform("view", Matrix4::GetModelMatrix(Camera::GetTransform()));
		shader->SetUniform("transform", Matrix4::GetModelMatrix(gameObject->transform));
		shader->SetUniform("lightPos", { 0, 0, -20 });
		shader->SetUniform("lightColor", Vector3<float>(1, 1, Math::Sin(Time::time)));
		shader->SetUniform("test", (float)((Math::Sin(Time::time * Renderer::test) + 1)) / 2);

		glDrawElements(GL_TRIANGLES, mesh->GetIndicesLength(), GL_UNSIGNED_INT, nullptr);

		gameObject->transform.rotation.x += Time::deltaTime * Renderer::test, 0.0f, 360.0f;
		gameObject->transform.rotation.x = Math::ToRange(gameObject->transform.rotation.x, 0.0f, 360.0f);
	}
}