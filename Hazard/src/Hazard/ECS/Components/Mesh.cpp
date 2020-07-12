#pragma once

#include <hzrpch.h>
#include "Mesh.h"

namespace Hazard {
	

	Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<int> _indices)
	{
		this->vertices = _vertices;
		this->indices = _indices;
	}


	Mesh::~Mesh()
	{
		indices.clear();
		vertices.clear();
	}
}