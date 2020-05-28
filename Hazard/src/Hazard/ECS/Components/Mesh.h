#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard {
	class HAZARD_API Mesh {
	public: 

		Mesh();
		~Mesh();

		float* GetVertices() { return vertices; };
		float* GetTextureCoords() { return textCoord; }
		int* GetIndices() { return indices; }


	private:
		
		float vertices[12] = {
			-1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f
		};
		float textCoord[8] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
		};

		int indices[6] = {
			0, 1, 2,
			0, 3, 2
		};
	};
}