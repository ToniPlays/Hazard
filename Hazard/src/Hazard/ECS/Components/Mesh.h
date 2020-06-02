#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Utils/Maths/Vector/Vector3.h"
#include "Hazard/ECS/Components/Vertex.h"

namespace Hazard {
	class HAZARD_API Mesh {
	public: 

		Mesh(std::vector<Vertex> vertices, std::vector<int> indices);
		~Mesh();
		
		float* GetVertices() { return vertices.data(); };
		float* GetNormals() { return normals.data(); };
		int* GetIndices() { return indices.data(); }

		int GetVerticesLength() { return vertices.size() / 3; };
		int GetIndicesLength() { return indices.size(); }

		bool HasNormals() { return hasNormals; }

		float* GetTextureCoords() { return textCoord.data(); }

	private:
		
		bool hasNormals = false;

		std::vector<float> vertices;
		std::vector<float> textCoord;
		std::vector<float> normals;
		std::vector<int> indices;
	};
}