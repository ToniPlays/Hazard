#pragma once
#include <hzrpch.h>
#include "MeshFactory.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Hazard::Rendering {

	uint32_t meshFlags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords 
		| aiProcess_OptimizeMeshes | aiProcess_ValidateDataStructure | aiProcess_JoinIdenticalVertices;

	std::vector<Mesh*> MeshFactory::loadedMeshes;

	Mesh* MeshFactory::LoadMesh(const std::string& file)
	{
		for (uint32_t i = 0; i < loadedMeshes.size(); i++) {
			if (loadedMeshes[i]->GetFile() == file)
				return loadedMeshes[i];
		}
		
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(file, meshFlags);

		if (!scene || !scene->HasMeshes()) {
			HZR_CORE_WARN("No meshes in {0}", file);
			return nullptr;
		}

		MeshData data;

		data.subMeshes.reserve(scene->mNumMeshes);
		ProcessNode(scene->mRootNode, scene, data);

		Material* material = Material::Create("res/shaders/PBRShader.glsl");
		Mesh* mesh = new Mesh(file, data.vertices, data.indices);
		mesh->SetMaterial(material);

		if (scene->HasMaterials())
			GetMaterials(scene, *material);

		loadedMeshes.push_back(mesh);
		return mesh;
	}
	
	void MeshFactory::ProcessNode(aiNode* node, const aiScene* scene, MeshData& data)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMesh(aiMesh, scene, data);
		}
		for (uint32_t i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene, data);
		}
	}

	void MeshFactory::ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& data)
	{
		SubMesh subMesh = data.subMeshes.emplace_back();

		subMesh.baseVertex = data.vertexIndex;
		subMesh.baseIndex = data.baseIndex;

		subMesh.materialIndex = mesh->mMaterialIndex;

		subMesh.vertexCount = mesh->mNumVertices;
		subMesh.indexCount = mesh->mNumFaces * 3;

		data.vertexIndex += subMesh.vertexCount;
		data.baseIndex += subMesh.indexCount;

		subMesh.meshName = mesh->mName.C_Str();

		uint32_t colors = 0;

		for (uint32_t i = 0; i < mesh->GetNumColorChannels(); i++) {
			if (mesh->HasVertexColors(i)) {
				colors = i;
				break;
			}
		}


		for (size_t i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;

			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;

			if (mesh->HasVertexColors(colors)) {
				vertex.color.r = mesh->mColors[i]->r;
				vertex.color.g = mesh->mColors[i]->g;
				vertex.color.b = mesh->mColors[i]->b;
				vertex.color.a = mesh->mColors[i]->a;
			}
			if (mesh->HasNormals()) {
				vertex.normals.x = mesh->mNormals[i].x;
				vertex.normals.y = mesh->mNormals[i].y;
				vertex.normals.z = mesh->mNormals[i].z;
			}
			if (mesh->HasTextureCoords(0)) {
				vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
				vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
			}
			data.vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				data.indices.push_back(face.mIndices[j] + subMesh.baseVertex);
		}

	}
	void MeshFactory::GetMaterials(const aiScene* scene, Material& material)
	{

		HZR_CORE_INFO("Loading materials");
		std::vector<Texture2D*> textures;
		textures.reserve(scene->mNumMaterials);

		for (uint16_t i = 0; i < scene->mNumMaterials; i++) {

			aiMaterial* aiMat = scene->mMaterials[i];

			LoadMaterialTexture(aiMat, material, aiTextureType_BASE_COLOR, "texture_albedo");
			LoadMaterialTexture(aiMat, material, aiTextureType_DIFFUSE   , "texture_diffuse");
			LoadMaterialTexture(aiMat, material, aiTextureType_SPECULAR  , "texture_specular");
			LoadMaterialTexture(aiMat, material, aiTextureType_AMBIENT   , "texture_ambient");
		}

		material.SetTextures(textures);
	}
	void MeshFactory::LoadMaterialTexture(aiMaterial* material, Material& mat, aiTextureType type, const char* typeName)
	{
		std::vector<Texture2D*> textures;

		HZR_CORE_INFO("Are there materials? {0}", material->GetTextureCount(type));

		for (uint32_t i = 0; i < material->GetTextureCount(type); i++) {
			aiString str;
			material->GetTexture(type, i, &str);

			HZR_CORE_INFO("Loading texture {0}", str.C_Str());
		}
	}
	Mesh* MeshFactory::LoadCube()
	{
		return LoadMesh("res/models/cube.obj");
	}
}