#pragma once
#include <hzrpch.h>
#include "MeshFactory.h"

#include <assimp/Importer.hpp>

namespace Hazard::Rendering {

	uint32_t meshFlags = aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_ValidateDataStructure | aiProcess_JoinIdenticalVertices;

	Mesh* MeshFactory::LoadMesh(const char* file)
	{

		HZR_CORE_INFO("Loading mesh {0}", file);
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(file, meshFlags);

		if (!scene || !scene->HasMeshes()) {
			HZR_CORE_WARN("No meshes in {0}", file);
			return nullptr;
		}
		
		for (uint16_t i; i < scene->mNumMeshes; i++) {
			aiMesh* aiMesh = scene->mMeshes[i];
			return ProcessMesh(aiMesh, scene);
		}
		return nullptr;
	}
	Mesh* MeshFactory::LoadCube()
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		vertices.resize(8);
		vertices[0].position = { -1.0f, -1.0f,  1.0f };
		vertices[1].position = {  1.0f, -1.0f,  1.0f };
		vertices[2].position = {  1.0f,  1.0f,  1.0f };
		vertices[3].position = { -1.0f,  1.0f,  1.0f };
		vertices[4].position = { -1.0f, -1.0f, -1.0f };
		vertices[5].position = {  1.0f, -1.0f, -1.0f };
		vertices[6].position = {  1.0f,  1.0f, -1.0f };
		vertices[7].position = { -1.0f,  1.0f, -1.0f };

		indices.resize(36);

		indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
		indices[3]  = 2; indices[4]  = 3; indices[5]  = 0;
		indices[6]  = 1; indices[7]  = 5; indices[8]  = 6;
		indices[9]  = 6; indices[10] = 2; indices[11] = 1;
		indices[12] = 7; indices[13] = 6; indices[14] = 5;
		indices[15] = 5; indices[16] = 4; indices[17] = 7;
		indices[18] = 4; indices[19] = 0; indices[20] = 3;
		indices[21] = 3; indices[22] = 7; indices[23] = 4;
		indices[24] = 4; indices[25] = 5; indices[26] = 1;
		indices[27] = 1; indices[28] = 0; indices[29] = 4;
		indices[30] = 3; indices[31] = 2; indices[32] = 6;
		indices[33] = 6; indices[34] = 7; indices[35] = 3;


		return new Mesh(vertices, indices);
	}
	Mesh* MeshFactory::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		for (uint16_t i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;

			if (mesh->HasPositions()) {
				vertex.position.x = mesh->mVertices[i].x;
				vertex.position.y = mesh->mVertices[i].y;
				vertex.position.z = mesh->mVertices[i].z;
			}
			if (mesh->HasTextureCoords(0)) {
				vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
				vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
			}
			if (mesh->HasVertexColors(0)) {
				vertex.color.r = mesh->mColors[i]->r;
				vertex.color.g = mesh->mColors[i]->g;
				vertex.color.b = mesh->mColors[i]->b;
				vertex.color.a = mesh->mColors[i]->a;
			}

			vertices.push_back(vertex);
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		return new Mesh(vertices, indices);
	}
}