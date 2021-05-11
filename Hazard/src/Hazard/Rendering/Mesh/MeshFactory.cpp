#pragma once
#include <hzrpch.h>
#include "MeshFactory.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Hazard/File/File.h"

namespace Hazard::Rendering {


	glm::mat4 AssimpMat4ToGlmMat4(const aiMatrix4x4& matrix)
	{
		glm::mat4 result;
		result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
		result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
		result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
		result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
		return result;
	}

	uint32_t meshFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords
		| aiProcess_OptimizeMeshes | aiProcess_ValidateDataStructure | aiProcess_JoinIdenticalVertices;

	std::vector<Mesh*> MeshFactory::m_LoadedMeshes;

	Mesh* MeshFactory::LoadMesh(const std::string& file)
	{
		std::string absoluteFile = File::GetFileAbsolutePath(file);

		for (uint32_t i = 0; i < m_LoadedMeshes.size(); i++) {
			if (m_LoadedMeshes[i]->GetFile() == file)
				return m_LoadedMeshes[i];
		}
		
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(absoluteFile, meshFlags);

		if (!scene || !scene->HasMeshes()) {
			HZR_CORE_WARN("No meshes in {0}", absoluteFile);
			return nullptr;
		}
		MeshData data;

		data.subMeshes.reserve(scene->mNumMeshes);
		ProcessNode(scene->mRootNode, scene, data);

		TraverseNode(scene->mRootNode, data);

		Material* material = Material::Create("res/shaders/PBRShader.glsl");
		Mesh* mesh = new Mesh(absoluteFile, data.vertices, data.indices);
		mesh->SetMaterial(material);

		if (scene->HasMaterials())
			GetMaterials(scene, *material, file.c_str());

		m_LoadedMeshes.push_back(mesh);
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
	void MeshFactory::TraverseNode(aiNode* node, MeshData& data, const glm::mat4 parentTransform, uint32_t level)
	{
		glm::mat4 local = AssimpMat4ToGlmMat4(node->mTransformation);
		glm::mat4 transform = parentTransform * local;

		for (uint32_t i = 0; i < node->mNumMeshes; i++) {
			
			uint32_t mesh = node->mMeshes[i];
			auto& submesh = data.subMeshes[i];

			submesh.nodeName = node->mName.C_Str();
			submesh.transform = transform;
			submesh.localTransform = local;
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++) {
			TraverseNode(node->mChildren[i], data, transform, ++level);
		}

	}
	void MeshFactory::GetMaterials(const aiScene* scene, Material& material, const char* path)
	{
		std::vector<Ref<Texture2D>> textures;
		textures.reserve(scene->mNumMaterials);

		for (uint16_t i = 0; i < scene->mNumMaterials; i++) {

			aiMaterial* aiMat = scene->mMaterials[i];

			LoadMaterialTexture(aiMat, material, aiTextureType_BASE_COLOR, "texture_albedo",   path);
			LoadMaterialTexture(aiMat, material, aiTextureType_DIFFUSE   , "texture_diffuse",  path);
			LoadMaterialTexture(aiMat, material, aiTextureType_SPECULAR  , "texture_specular", path);
			LoadMaterialTexture(aiMat, material, aiTextureType_AMBIENT   , "texture_ambient",  path);
		}

		material.SetTextures(textures);
	}
	void MeshFactory::LoadMaterialTexture(aiMaterial* material, Material& mat, aiTextureType type, const char* typeName, const char* path)
	{
		std::vector<Texture2D*> textures;
		aiString aiTexPath;


		for (uint32_t i = 0; i < material->GetTextureCount(type); i++) {

			if (material->GetTexture(type, i, &aiTexPath) != AI_SUCCESS) continue;

			std::filesystem::path _path = path;
			auto parentPath = _path.parent_path();
			parentPath /= std::string(aiTexPath.data);
			std::string texturePath = parentPath.string();

			HZR_CORE_INFO("Getting " + std::string(typeName) + " from " + parentPath.string());

			//RenderUtils::Create<Texture2D>(texturePath.c_str());
		}
	}
	Mesh* MeshFactory::LoadCube()
	{
		return LoadMesh("res/models/cube.obj");
	}
	Mesh* MeshFactory::LoadSphere()
	{
		return LoadMesh("res/models/ico.obj");
	}
	Mesh* MeshFactory::LoadPlane()
	{
		return LoadMesh("res/models/plane.obj");
	}
}