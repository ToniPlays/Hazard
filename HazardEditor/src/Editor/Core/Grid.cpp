#pragma once

#include "Grid.h"

Grid::~Grid()
{

}
void Grid::OnEnabled()
{
	shader = RendererAPI::Shader("res/shaders/grid.glsl");
	vertexArray = RendererAPI::VertexArray();

	VertexBuffer* points = RendererAPI::VertexBuffer(ShaderDataType::Float3, "points");
	float p[12] = {
		-1.0f, 0.0f,  1.0f, // top-left
		 1.0f, 0.0f,  1.0f, // top-right
		 1.0f, 0.0f, -1.0f, // bottom-right
		-1.0f, 0.0f, -1.0f  // bottom-left
	};

	points->SetData(p, 8);
	vertexArray->SetLayout({ points });
}
void Grid::Render()
{
	
	vertexArray->Bind();
	shader->Bind();
	shader->SetUniform("projection", Hazard::Renderer::GetProjection());
	shader->SetUniform("view", Hazard::Matrix4::GetModelMatrix(Hazard::Camera::GetTransform()));
	shader->SetUniform("transform", Hazard::Matrix4::GetModelMatrix(Hazard::Transform()));

	glDrawArrays(GL_POINTS, 0, 4);
}

