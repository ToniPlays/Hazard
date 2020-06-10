#pragma once

#include <hzrpch.h>
#include "Mesh.h"

namespace Hazard {
	

	Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<int> _indices)
	{
		for (Vertex v : _vertices) {
			this->vertices.push_back(v.position.x);
			this->vertices.push_back(v.position.y);
			this->vertices.push_back(v.position.z);

			this->textCoord.push_back(v.textureCoords.x);
			this->textCoord.push_back(v.textureCoords.y);
			
			this->normals.push_back(v.normal.x);
			this->normals.push_back(v.normal.y);
			this->normals.push_back(v.normal.z);
		}
		this->indices = _indices;

		std::cout << "Mesh: " << _vertices.size() << " v, indices " << indices.size() << std::endl;
	}


	Mesh::~Mesh()
	{
		indices.clear();
		vertices.clear();
	}
}