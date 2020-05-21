#pragma once
#include <hzrpch.h>
#include "GlobalRenderer.h"

namespace Hazard {

	GlobalRenderer::GlobalRenderer() : Module("GlobalRenderer")
	{
		float vertices[12] = {
			-0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f
		};

		float colors[12] = {
			 6 / 255.0f,  38 / 255.0f, 111 / 255.0f,
			20 / 255.0f, 115 / 255.0f, 198 / 255.0f,
			20 / 255.0f, 115 / 255.0f, 198 / 255.0f,
			 6 / 255.0f,  38 / 255.0f, 111 / 255.0f
		};
		int indices[2 * 3] = {
			0, 1, 2,
			0, 2, 3
		};
		//Gen vertex
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		//VertexBuffer position data
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
		//Color buffer data

		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);

		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

		//Indices
		glGenBuffers(1, &indiceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		defaultTarget = new DefaultRenderTexture();

		shader = new Shader(
			R"(
			#version 330 core

			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 vertexColor;
			out vec4 fragColor;

			void main() {
				gl_Position = vec4(position, 1.0);
				fragColor = vertexColor;
			}
			)", R"(
				#version 330 core
				in vec4 fragColor;
				out vec4 color;

				void main() {
					color = fragColor;
				}
			)"
		);
	}

	void GlobalRenderer::SceneRender()
	{
		//Clear and set FBO
		Window& window = Application::GetCurrent().GetWindow();
		//FBO
		RenderTexture* texture = GetRenderTexture();
		texture->Bind();
		if(texture->GetID() != 0) 
			window.GetContext()->SetViewport(0, 0, texture->GetWidth(), texture->GetHeight());
		//CLEAR COLOR
		texture->Clear();
		//Set default FBO
		window.GetContext()->SetViewport(0, 0, window.GetWidth(), window.GetHeight());

		shader->Bind();

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);	

		shader->Unbind();
		defaultTarget->Bind();
	}
}
