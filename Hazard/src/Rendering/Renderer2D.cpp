#pragma once
#include <hzrpch.h>
#include "Renderer2D.h"

#include "glad/glad.h"

namespace Hazard {
	Renderer2D::Renderer2D() {

		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		float vertices[4 * 3] = {
			-0.4f, -0.2f, 0.0f,
			-0.4f, 0.6f, 0.0f,
			0.4f, 0.6f, 0.0f,
			0.4f, -0.2f, 0.0f
		};
		int indices[6] = {
			0, 1, 2,
			0, 3, 2
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}
	Renderer2D::~Renderer2D() 
	{
	
	}
	void Renderer2D::Render() 
	{

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	}
	void Renderer2D::RenderToTexture()
	{
		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	}
}