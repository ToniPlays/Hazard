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

		while (std::getline(ifs, line)) {
			std::string token = line.substr(0, line.find_first_of(" "));

			if (token == "v") positions.push_back(GetPos(line));
			else if (token == "vn") normals.push_back(GetPos(line));
			else if (token == "f") break;
		}

		std::map<std::string, Vertex> vertices;
		std::vector<int> indices;
		std::list<std::string> insertion;

		int offset = 0;

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
				if (vertices.count(temp) == 0) {
					vertices[temp] = Vertex(positions.at(v), Vector3<float>::Invert(normals.at(vn)));
					insertion.push_back(temp);
				}
				else {
					/*std::stringstream ss;
					ss << temp << "_" << offset;
					vertices[ss.str()] = Vertex(positions.at(v), Vector3<float>::Invert(normals.at(vn)));
					//offset++;*/
				}
				indices.push_back(v + offset);
			}

		} while (std::getline(ifs, line));

		std::vector<Vertex> v;
		std::vector<std::string> keys;

		for (std::pair<std::string, Vertex> p : vertices) {
			keys.push_back(p.first);
		}
		std::cout << "Vertices " << keys.size() << std::endl;
		for (std::string index : keys) {
			std::cout << "Vertex " << vertices[index].position << ", " << vertices[index].normal << std::endl;
			v.push_back(vertices[index]);
		}

		std::cout << "Indices " << indices.size();
		int c = 0;
		for (int i : indices) {
			if (c % 3 == 0) std::cout << "\nTriangle: ";
			std::cout << i << "/";
			c++;
		}
		std::cout << "\n";

		return new Mesh(v, indices);
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
		vertices.push_back(Vertex({ -1, 1, 1 }, { 0, 0, -1 }));
		vertices.push_back(Vertex({ -1,-1, 1 }, { 0, 0, -1 }));
		vertices.push_back(Vertex({ 1,-1, 1 }, { 0, 0, -1 }));
		vertices.push_back(Vertex({ 1, 1, 1 }, { 0, 0, -1 }));
		indices.insert(indices.end(), { 0, 1, 2 });
		indices.insert(indices.end(), { 0, 2, 3 });
		//Back
		vertices.push_back(Vertex({ -1, 1,-1 }, { 0, 0, 1 }));
		vertices.push_back(Vertex({ -1,-1,-1 }, { 0, 0, 1 }));
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
		vertices.push_back(Vertex({ -1, 1,-1 }, { 0, -1, 0 }));
		vertices.push_back(Vertex({ -1, 1, 1 }, { 0, -1, 0 }));
		vertices.push_back(Vertex({ 1, 1, 1 }, { 0, -1, 0 }));
		vertices.push_back(Vertex({ 1, 1,-1 }, { 0, -1, 0 }));
		indices.insert(indices.end(), { 16, 17, 18 });
		indices.insert(indices.end(), { 16, 18, 19 });
		//Bottom
		vertices.push_back(Vertex({ -1,-1,-1 }, { 0, 1, 0 }));
		vertices.push_back(Vertex({ -1,-1, 1 }, { 0, 1, 0 }));
		vertices.push_back(Vertex({ 1,-1, 1 }, { 0, 1, 0 }));
		vertices.push_back(Vertex({ 1,-1,-1 }, { 0, 1, 0 }));
		indices.insert(indices.end(), { 20, 21, 22 });
		indices.insert(indices.end(), { 20, 23, 22 });



		std::cout << "Vertices " << vertices.size() << ", indices " << indices.size() << std::endl;

		return new Mesh(vertices, indices);
	}
	Vector3<float> MeshLoader::GetPos(std::string line) {
		std::string token = line.substr(0, line.find_first_of(" "));
		line = line.substr(std::strlen(token.c_str()), line.find_first_of("\r\n"));
		size_t pos = 0;
		std::string l = line.substr(pos, line.find_first_of(" ", pos + 1));
		float x = std::stof(l);

		pos += std::strlen(l.c_str());
		l = line.substr(pos, line.find_first_of(" ", pos));
		float y = std::stof(l);

		pos += std::strlen(l.c_str());
		l = line.substr(pos, line.find_first_of(" ", pos));
		float z = std::stof(l);

		return Vector3<float>(x, y, z);
	}
}

