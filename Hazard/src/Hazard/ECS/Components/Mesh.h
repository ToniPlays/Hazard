#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/ECS/Components/Vertex.h"

namespace Hazard {
	class HAZARD_API Mesh {
	public: 

		Mesh(std::vector<Vertex> vertices, std::vector<int> indices);
		~Mesh();
		
		std::vector<Vertex> GetVertices() { return vertices; };

		size_t GetVerticesLength() { return vertices.size(); };
		size_t GetIndicesLength() { return indices.size(); }

		std::vector<int> GetIndices() { return indices; }

		bool HasNormals() { return hasNormals; }

	private:
		
		bool hasNormals = false;

		std::vector<Vertex> vertices;
		std::vector<int> indices;
	};
}