#include "hzrpch.h"
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

	}

	void GlobalRenderer::SceneRender()
	{
		//Clear and set FBO
		//FBO
		renderTarget->Bind();
		//CLEAR COLOR
		Color c = renderTarget->GetClearColor();
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(c.r, c.g, c.b, c.a);

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);		
		//Set default FBO
		defaultTarget->Bind();
	}
}
