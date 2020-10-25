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

		HZR_CORE_INFO("Loading mesh: " + file);
		std::ifstream ifs(file);

		std::vector<Vector3<float>> positions;
		std::vector<Vector2<float>> txC;
		std::vector<Vector3<float>> normals;
		std::string line;
		int offset = 0;

		while (std::getline(ifs, line)) {
			std::string token = line.substr(0, line.find_first_of(" "));

			if (token == "v") positions.push_back(GetPosV3(line));
			else if (token == "vt") txC.push_back(GetPosV2(line));
			else if (token == "vn") normals.push_back(GetPosV3(line));
			else if (token == "f") break;
		}

		std::map<std::string, Vertex> vertices;
		std::vector<int> indices;
		std::list<std::string> insertion;


		HZR_CORE_INFO("Loading faces");
		
		do {

			std::string token = line.substr(0, line.find_first_of(" "));
			line = line.substr(std::strlen(token.c_str()) + 1, line.find_first_of("\r\n"));

			if (token != "f") continue;

			std::stringstream ss(line);
			std::string temp;


			while (std::getline(ss, temp, ' ')) {
				int v, vt, vn;

				GetData(temp, v, vt, vn);
				std::stringstream vOffset;

				vOffset << temp;

				if (vertices.count(vOffset.str()) > 1) {
					vOffset << "_" << offset;
					offset++;
				}

				vertices[vOffset.str()] = Vertex(positions.at(v), txC.at(vt), Vector3<float>::Invert(normals.at(vn)));
				indices.push_back(v + offset);
				insertion.push_back(vOffset.str());
			}

		} while (std::getline(ifs, line));

		std::cout << "\nSorting:\n" << std::endl;
		std::vector<Vertex> v;
		std::vector<std::string> keys;

		for (std::pair<std::string, Vertex> p : vertices) {
			keys.push_back(p.first);
		}
		int tempCount = 1;
		for (int ind : indices) {
			std::cout << ind;
			if (tempCount % 3 == 0) std::cout << "\n";
			else std::cout << "/";
			tempCount++;
		}

		std::cout << "\n\nVertices " << keys.size() << std::endl;
		for (std::string index : keys) {
			//std::cout << "Vertex (" << index << ") pos " << vertices[index].position << ", vn" << vertices[index].normal << ", vt " << 
			//	vertices[index].textureCoords << std::endl;
			v.push_back(vertices[index]);
		}

		return new Mesh();
	}

	void MeshLoader::GetData(std::string line, int& v, int& vt, int& vn)
	{
		std::stringstream ss(line);
		std::string temp;

		while (std::getline(ss, temp, '/')) {
			v = std::stoi(temp) - 1;
			break;
		}
		while (std::getline(ss, temp, '/')) {
			vt = std::stoi(temp) - 1;
			break;
		}
		while (std::getline(ss, temp, '/')) {
			vn = std::stoi(temp) - 1;
			break;
		}
	}

	Mesh* MeshLoader::LoadCube()
	{
		std::vector<Vertex> vertices;
		std::vector<int> indices;

		//Cube front
		vertices.push_back(Vertex({-1, 1, 1 }, { 0, 1 }, { 0, 0, -1 }));
		vertices.push_back(Vertex({-1,-1, 1 }, { 1, 0 }, { 0, 0, -1 }));
		vertices.push_back(Vertex({ 1,-1, 1 }, { 1, 1 }, { 0, 0, -1 }));
		vertices.push_back(Vertex({ 1, 1, 1 }, { 0, 0 }, { 0, 0, -1 }));
		indices.insert(indices.end(), { 0, 1, 2 });
		indices.insert(indices.end(), { 0, 2, 3 });
		//Back
		vertices.push_back(Vertex({-1, 1,-1 }, { 0, 1 }, { 0, 0, 1 }));
		vertices.push_back(Vertex({-1,-1,-1 }, { 0, 1 }, { 0, 0, 1 }));
		vertices.push_back(Vertex({ 1,-1,-1 }, { 1, 1 }, { 0, 0, 1 }));
		vertices.push_back(Vertex({ 1, 1,-1 }, { 1, 0 }, { 0, 0, 1 }));
		indices.insert(indices.end(), { 4, 5, 6 });
		indices.insert(indices.end(), { 4, 6, 7 });
		//Left
		vertices.push_back(Vertex({-1, 1, 1 }, { 0, 1 }, { 1, 0, 0 }));
		vertices.push_back(Vertex({-1,-1, 1 }, { 0, 1 }, { 1, 0, 0 }));
		vertices.push_back(Vertex({-1, 1,-1 }, { 1, 1 }, { 1, 0, 0 }));
		vertices.push_back(Vertex({-1,-1,-1 }, { 1, 0 }, { 1, 0, 0 }));
		indices.insert(indices.end(), { 8, 9, 10 });
		indices.insert(indices.end(), { 9, 10, 11 });
		//Right
		vertices.push_back(Vertex({ 1, 1, 1 }, { 0, 1 }, { -1, 0, 0 }));
		vertices.push_back(Vertex({ 1,-1, 1 }, { 0, 1 }, { -1, 0, 0 }));
		vertices.push_back(Vertex({ 1, 1,-1 }, { 1, 1 }, { -1, 0, 0 }));
		vertices.push_back(Vertex({ 1,-1,-1 }, { 1, 0 }, { -1, 0, 0 }));
		indices.insert(indices.end(), { 12, 13, 14 });
		indices.insert(indices.end(), { 13, 14, 15 });
		//Top
		vertices.push_back(Vertex({-1, 1,-1 }, { 0, 1 }, { 0, -1, 0 }));
		vertices.push_back(Vertex({-1, 1, 1 }, { 0, 1 }, { 0, -1, 0 }));
		vertices.push_back(Vertex({ 1, 1, 1 }, { 1, 1 }, { 0, -1, 0 }));
		vertices.push_back(Vertex({ 1, 1,-1 }, { 1, 0 }, { 0, -1, 0 }));
		indices.insert(indices.end(), { 16, 17, 18 });
		indices.insert(indices.end(), { 16, 18, 19 });
		//Bottom
		vertices.push_back(Vertex({-1,-1,-1 }, { 1, 1 }, { 0, 1, 0 }));
		vertices.push_back(Vertex({-1,-1, 1 }, { 0, 1 }, { 0, 1, 0 }));
		vertices.push_back(Vertex({ 1,-1, 1 }, { 1, 1 }, { 0, 1, 0 }));
		vertices.push_back(Vertex({ 1,-1,-1 }, { 1, 0 }, { 0, 1, 0 }));
		indices.insert(indices.end(), { 20, 21, 22 });
		indices.insert(indices.end(), { 20, 23, 22 });
		
		//std::cout << "Vertices " << vertices.size() << ", indices " << indices.size() << std::endl;

		return new Mesh();
	}

	Mesh* MeshLoader::LoadPlane()
	{
		std::vector<Vertex> vertices;
		std::vector<int> indices;

		//Cube front
		vertices.push_back(Vertex({ -1, 1, 1 }, { 0, 1 }, { 0, 0, -1 }));
		vertices.push_back(Vertex({ -1,-1, 1 }, { 1, 0 }, { 0, 0, -1 }));
		vertices.push_back(Vertex({ 1,-1, 1 }, { 1, 1 }, { 0, 0, -1 }));
		vertices.push_back(Vertex({ 1, 1, 1 }, { 0, 0 }, { 0, 0, -1 }));
		indices.insert(indices.end(), { 0, 1, 2 });
		indices.insert(indices.end(), { 0, 2, 3 });

		return new Mesh();
	}

	Vector3<float> MeshLoader::GetPosV3(std::string line) {
		std::stringstream ss;
		ss << line;
		std::string current;
		std::vector<float> result;
		bool f = false;

		while (std::getline(ss, current, ' ')) {
			if (!f) {
				f = true;
				continue;
			}
			result.push_back(std::stof(current));
		}
		return Vector3<float>(result.at(0), result.at(1), result.at(2));
	}

	Vector2<float> MeshLoader::GetPosV2(std::string line)
	{
		std::string token = line.substr(0, line.find_first_of(" "));
		line = line.substr(std::strlen(token.c_str()), line.find_first_of("\r\n"));
		size_t pos = 0;
		std::string l = line.substr(pos, line.find_first_of(" ", pos + 1));
		float x = std::stof(l);

		pos += std::strlen(l.c_str());
		l = line.substr(pos, line.find_first_of(" ", pos));
		float y = std::stof(l);

		return Vector2<float>(x, 1 - y);
	}

}

