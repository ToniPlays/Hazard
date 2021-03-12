#pragma once

#include "VertexData.h"

namespace Hazard::Rendering {


	class Mesh {
	public:
		Mesh() = default;
		Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
		~Mesh();

		void Render();

		Shader& GetShader() { return *shader; }
		void GenerateArrays();

	private:
		
	private:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		Shader* shader;

		VertexArray* meshVAO;
	};
}