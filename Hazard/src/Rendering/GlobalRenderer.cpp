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
		int indices[2 * 3] = {
			0, 1, 2,
			0, 2, 3
		};

		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

		glGenBuffers(1, &indiceBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		defaultTarget = new DefaultRenderTexture();

		shader = new Shader(
			R"(
			#version 330 core

			layout(location = 0) in vec3 position;
			out vec4 fragColor;

			void main() {
				gl_Position = vec4(position, 1.0);
				fragColor = vec4(position + 0.5, 1.0);
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
		HZR_CORE_INFO("Shader program" + shader->GetProgram());
	}

	void GlobalRenderer::SceneRender()
	{
		//Clear and set FBO
		Window& window = Application::GetCurrent().GetWindow();
		//FBO
		RenderTexture* texture = GetRenderTexture();
		texture->Bind();
		if(texture->GetID() != 0) window.GetContext()->SetViewport(0, 0, texture->GetWidth(), texture->GetHeight());
		//CLEAR COLOR
		Color c = texture->GetClearColor();
		glClearColor(c.r, c.g, c.b, c.a);
		glClear(GL_COLOR_BUFFER_BIT);
		//Set default FBO
		window.GetContext()->SetViewport(0, 0, window.GetWidth(), window.GetHeight());
		shader->Bind();

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);		
		shader->Unbind();
		defaultTarget->Bind();
	}
}
