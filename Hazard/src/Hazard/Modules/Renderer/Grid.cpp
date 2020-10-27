#pragma once
#include <hzrpch.h>
#include "RendererAPI.h"
#include "RenderEngine.h"
#include "Grid.h"

namespace Hazard {

	Grid::Grid()
	{

		shader = RendererAPI::Shader("res/shaders/grid.glsl");
		vertexArray = RendererAPI::VertexArray();

		VertexBuffer* points = RendererAPI::VertexBuffer(ShaderDataType::Float3, "points");
		std::vector<Vector3<float>> p;

		int gridSize = 10;

		for (int y = -gridSize / 2; y < gridSize / 2; y++) {
			for (int x = -gridSize / 2; x < gridSize / 2; x++) {
				p.push_back(Vector3<float>(x, 0.0f, y));
				p.push_back(Vector3<float>(x, 0.0f, y + 1));
				p.push_back(Vector3<float>(x + 1, 0.0f, y + 1));
				p.push_back(Vector3<float>(x + 1, 0.0f, y));
			}
		}

		size = p.size();
		points->SetData(p.data(), size);
		vertexArray->SetLayout({ points });
	}


	Grid::~Grid()
	{

	}
	void Grid::Render()
	{
		/*vertexArray->BindAll();
		shader->Bind();

		shader->SetUniform("projection", RenderEngine::Instance->Get3DProjection());
		shader->SetUniform("view", Hazard::Matrix4::GetModelMatrix(Camera::GetTransform()));
		shader->SetUniform("transform", Hazard::Matrix4::GetModelMatrix(Hazard::Transform()));
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_QUADS, 0, size);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
	}
}
