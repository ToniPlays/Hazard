
#include <hzrpch.h>
#include "MeshFactory.h"
#include "Hazard/Assets/AssetManager.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

	Ref<Mesh> MeshFactory::LoadMesh(MeshCreateInfo* createInfo)
	{
		if (createInfo->VertexBuffer || createInfo->IndexBuffer) {
			Ref<Mesh> mesh = Ref<Mesh>::Create(createInfo->VertexBuffer, createInfo->IndexBuffer, nullptr);
			AssetManager::AddRuntimeResource(mesh);
			return mesh;
		}

		Timer timer;
		std::string absoluteFile = File::GetFileAbsolutePath(createInfo->Path);

		HZR_CORE_ASSERT(File::Exists(absoluteFile), "Mesh file does not exist");

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
		HZR_CORE_INFO("[MeshFactory]: Loading mesh {0} took {1} ms", createInfo->Path, timer.ElapsedMillis());

		Ref<Mesh> mesh = Ref<Mesh>::Create(absoluteFile, data.vertices, data.indices);
		AssetManager::AddRuntimeResource(mesh);
		return Ref<Mesh>::Create(absoluteFile, data.vertices, data.indices);
	}

	Ref<Mesh> MeshFactory::LoadCube()
	{
		MeshCreateInfo info = {};
		info.Path = "res/mesh/cube.obj";
		return LoadMesh(&info);
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
			Vertex3D vertex;
			vertex.Position.x = mesh->mVertices[i].x;
			vertex.Position.y = mesh->mVertices[i].y;
			vertex.Position.z = mesh->mVertices[i].z;

			if (mesh->HasVertexColors(colors)) {
				aiColor4D color = mesh->mColors[colors][i];
				vertex.Color.r = color.r;
				vertex.Color.g = color.g;
				vertex.Color.b = color.b;
				vertex.Color.a = color.a;
			}
			if (mesh->HasNormals()) {
				vertex.Normals.x = mesh->mNormals[i].x;
				vertex.Normals.y = mesh->mNormals[i].y;
				vertex.Normals.z = mesh->mNormals[i].z;
			}
			if (mesh->HasTextureCoords(0)) {
				vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
				vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
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
}
