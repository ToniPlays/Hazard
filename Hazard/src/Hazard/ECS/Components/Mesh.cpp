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
			
			this->normals.push_back(v.normal.x);
			this->normals.push_back(v.normal.y);
			this->normals.push_back(v.normal.z);
		}
		this->indices = _indices;
	}

	Mesh::~Mesh()
	{
		indices.clear();
		vertices.clear();
	}

}