#pragma once

#include <hzrpch.h>
#include "Renderer2D.h"
#include "Hazard//Modules/Renderer/GlobalRenderer.h"
#include "Hazard/ECS/Camera.h"
#include "Hazard/ECS/Components/SpriteRenderer.h"

namespace Hazard {

	int Renderer2D::size = 707;

	Mesh* CreateQuad(float x, float y, float size = 1) {
		
		std::vector<Vertex> vertices;
		vertices.push_back(Vertex({ x,        y,        0 }, { 0, 0 }, { 0, 0, 0 }));
		vertices.push_back(Vertex({ x,        y + size, 0 }, { 0, 0 }, { 0, 0, 0 }));
		vertices.push_back(Vertex({ x + size, y + size, 0 }, { 0, 0 }, { 0, 0, 0 }));
		vertices.push_back(Vertex({ x + size, y,        0 }, { 0, 0 }, { 0, 0, 0 }));

		std::vector<int> indices = {0, 1, 2, 0, 2, 3};

		return new Mesh(vertices, indices);
	}


	struct Data {
		VertexArray* vertexArray;
		Shader* shader;
	};
	Data data;

	std::vector<GameObject*> objects;

	Renderer2D::Renderer2D()
	{
		data.vertexArray = RendererAPI::VertexArray();
		data.vertexArray->SetIndexBuffer(RendererAPI::IndexBuffer());

		data.vertexArray->AddBuffer(RendererAPI::VertexBuffer(ShaderDataType::Float2, "position"));
		data.vertexArray->AddBuffer(RendererAPI::VertexBuffer(ShaderDataType::Float4, "color"));
		data.shader = RendererAPI::Shader("res/shaders/canvasShader.glsl");

		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++) {

				GameObject* go = new GameObject("OBJ");
				go->AddComponent(new SpriteRenderer(go, CreateQuad(x , y, 0.8f)));
				objects.push_back(go);
			}
		}
	}	

	Renderer2D::~Renderer2D()
	{

	}

	void Renderer2D::Render()
	{
		BeginScene();

		std::vector<float> positions = std::vector<float>();
		std::vector<float> colors = std::vector<float>();
		std::vector<int> indices = std::vector<int>();

		int offset = 0;

		for (GameObject* obj : objects) {
			SpriteRenderer* sprt = obj->GetComponent<SpriteRenderer>();

			for (Vertex v : sprt->GetMesh().GetVertices()) {

				positions.push_back(v.position.x);
				positions.push_back(v.position.y);

				colors.push_back(v.position.x / (float)size);
				colors.push_back(v.position.y / (float)size);

				colors.push_back(1.0);
				colors.push_back(1.0);
			}
			for (int i : sprt->GetMesh().GetIndices()) {
				indices.push_back(i + offset);
			}
			offset += sprt->GetMesh().GetVerticesLength();
		}

		data.vertexArray->GetBuffer("position")->SetData(positions.data(), positions.size() / 2);
		data.vertexArray->GetBuffer("color")->SetData(colors.data(), colors.size() / 4);
		data.vertexArray->GetIndexBuffer()->SetData(indices.data(), indices.size());
		
		EndScene();
	}

	void Renderer2D::OnResized(Event& e)
	{

	}

	void Renderer2D::BeginScene()
	{
		data.vertexArray->BindAll();
		data.shader->Bind();

		data.shader->SetUniform("projection", GlobalRenderer::Instance->Get3DProjection());
		data.shader->SetUniform("view", Matrix4::GetModelMatrix(Camera::GetTransform()));
		data.shader->SetUniform("transform", Matrix4::GetModelMatrix(Transform()));
	}

	void Renderer2D::EndScene()
	{
		GlobalRenderer::Draw(data.vertexArray);
		
	}
}