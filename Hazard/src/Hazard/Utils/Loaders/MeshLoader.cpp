#pragma once

#include <hzrpch.h>
#include "MeshLoader.h"
#include "Hazard/Utils/Maths/Vector/Vector3.h"
#include "Hazard/ECS/Components/Vertex.h"

namespace Hazard {

	Mesh* MeshLoader::LoadMesh(std::string file)
	{
		std::string ext = File::GetFileExtension(file);
		if (ext == "obj") return LoadObj(file);
		else return nullptr;
	}

	Mesh* MeshLoader::LoadObj(std::string file)
	{
		std::ifstream ifs(file);

		std::vector<Vertex> vertices;
		std::vector<int> indices;
		//Plane
		/*vertices.push_back(Vertex({ 0, 1, 1 }, { 1, 0, 0 }));
		vertices.push_back(Vertex({ 0,-1, 1 }, { 1, 0, 0 }));
		vertices.push_back(Vertex({ 0, 1,-1 }, { 1, 0, 0 }));
		vertices.push_back(Vertex({ 0,-1,-1 }, { 1, 0, 0 }));

		indices.insert(indices.end(), { 1, 2, 0 });
		indices.insert(indices.end(), { 1, 3, 2 });*/

		//Cube front
		vertices.push_back(Vertex({-1, 1, 1}, { 0, 0, -1}));
		vertices.push_back(Vertex({-1,-1, 1}, { 0, 0, -1}));
		vertices.push_back(Vertex({ 1,-1, 1 }, {0, 0, -1}));
		vertices.push_back(Vertex({ 1, 1, 1}, { 0, 0, -1}));
		indices.insert(indices.end(), { 0, 1, 2 });
		indices.insert(indices.end(), { 0, 2, 3 });
		//Back
		vertices.push_back(Vertex({-1, 1,-1 }, { 0, 0, 1 }));
		vertices.push_back(Vertex({-1,-1,-1 }, { 0, 0, 1 }));
		vertices.push_back(Vertex({ 1,-1,-1 }, { 0, 0, 1 }));
		vertices.push_back(Vertex({ 1, 1,-1 }, { 0, 0, 1 }));
		indices.insert(indices.end(), { 4, 5, 6 });
		indices.insert(indices.end(), { 4, 6, 7 });
		//Left
		vertices.push_back(Vertex({ -1, 1, 1 }, { 1, 0, 0 }));
		vertices.push_back(Vertex({ -1,-1, 1 }, { 1, 0, 0 }));
		vertices.push_back(Vertex({ -1, 1,-1 }, { 1, 0, 0 }));
		vertices.push_back(Vertex({ -1,-1,-1 }, { 1, 0, 0 }));
		indices.insert(indices.end(), { 8, 9, 10 });
		indices.insert(indices.end(), { 9, 10, 11 });
		//Right
		vertices.push_back(Vertex({ 1, 1, 1 }, { -1, 0, 0 }));
		vertices.push_back(Vertex({ 1,-1, 1 }, { -1, 0, 0 }));
		vertices.push_back(Vertex({ 1, 1,-1 }, { -1, 0, 0 }));
		vertices.push_back(Vertex({ 1,-1,-1 }, { -1, 0, 0 }));
		indices.insert(indices.end(), { 12, 13, 14 });
		indices.insert(indices.end(), { 13, 14, 15 });
		//Top
		vertices.push_back(Vertex({-1, 1,-1 }, { 0, -1, 0 }));
		vertices.push_back(Vertex({-1, 1, 1 }, { 0, -1, 0 }));
		vertices.push_back(Vertex({ 1, 1, 1 }, { 0, -1, 0 }));
		vertices.push_back(Vertex({ 1, 1,-1 }, { 0, -1, 0 }));
		indices.insert(indices.end(), { 16, 17, 18 });
		indices.insert(indices.end(), { 16, 18, 19 });
		//Bottom
		vertices.push_back(Vertex({-1,-1,-1 }, { 0, 1, 0 }));
		vertices.push_back(Vertex({-1,-1, 1 }, { 0, 1, 0 }));
		vertices.push_back(Vertex({ 1,-1, 1 }, { 0, 1, 0 }));
		vertices.push_back(Vertex({ 1,-1,-1 }, { 0, 1, 0 }));
		indices.insert(indices.end(), { 20, 21, 22 });
		indices.insert(indices.end(), { 20, 23, 22 });

		return new Mesh(vertices, indices);
	}

}
