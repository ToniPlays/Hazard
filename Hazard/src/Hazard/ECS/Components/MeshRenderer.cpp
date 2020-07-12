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

		std::vector<float> pos;
		std::vector<float> textCoords;
		std::vector<float> normals;

		for (Vertex v : mesh->GetVertices()) {

			pos.push_back(v.position.x);
			pos.push_back(v.position.y);
			pos.push_back(v.position.z);

			textCoords.push_back(v.textureCoords.x);
			textCoords.push_back(v.textureCoords.y);

			normals.push_back(v.normal.x);
			normals.push_back(v.normal.y);
			normals.push_back(v.normal.z);
		}

		vertexArray = RendererAPI::VertexArray();

		VertexBuffer* vertexBuffer = RendererAPI::VertexBuffer(ShaderDataType::Float3, "position");
		vertexBuffer->SetData(pos.data(), mesh->GetVerticesLength());

		VertexBuffer* textureCoords = RendererAPI::VertexBuffer(ShaderDataType::Float3, "position");
		textureCoords->SetData(textCoords.data(), mesh->GetVerticesLength());


		VertexBuffer* normalBuffer = RendererAPI::VertexBuffer(ShaderDataType::Float3, "normals");
		normalBuffer->SetData(normals.data(), mesh->GetVerticesLength());

		vertexArray->SetLayout({ vertexBuffer, textureCoords, normalBuffer });
		IndexBuffer* indexBuffer = RendererAPI::IndexBuffer();

		indexBuffer->SetData(mesh->GetIndices().data(), mesh->GetIndicesLength());
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
		vertexArray->BindAll();
		shader->Bind();
		texture->Bind();

		shader->SetUniform("projection", GlobalRenderer::Instance->Get3DProjection());
		shader->SetUniform("view", Matrix4::GetModelMatrix(Camera::GetTransform()));
		shader->SetUniform("transform", Matrix4::GetModelMatrix(gameObject->transform));
		shader->SetUniform("lightPos", { 0, 0, -20 });
		shader->SetUniform("lightColor", Vector3<float>(1, 1, 1));
		shader->SetUniform("test", (float)((Math::Sin(Time::time) + 1)) / 2);

		GlobalRenderer::Draw(vertexArray);

		gameObject->transform.rotation.y += Time::deltaTime * 25.0f;
	}
}