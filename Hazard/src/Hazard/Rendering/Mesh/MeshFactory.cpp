
#include <hzrpch.h>
#include "MeshFactory.h"

#define ASSIMP_LOG_INFO(x) HZR_CORE_INFO(x);
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Hazard
{
	glm::mat4 AssimpMat4ToGlmMat4(const aiMatrix4x4& matrix)
	{
		glm::mat4 result = {};
		result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
		result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
		result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
		result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
		return result;
	}

	void MeshFactory::SetOptimization(MeshLoaderFlags flags)
	{
		m_MeshFlags = flags;
	}

	uint32_t MeshFactory::GetColorChannel(const aiMesh* mesh)
	{
		for (uint32_t i = 0; i < mesh->GetNumColorChannels(); i++)
		{
			if (mesh->HasVertexColors(i))
				return i;
		}
		return UINT32_MAX;
	}

	uint64_t MeshFactory::GetVertexSize(const MeshData& data)
	{
		uint32_t vertexSize = 0;
		vertexSize += data.Flags & MeshFlags_Positions ? sizeof(glm::vec3) : 0;
		vertexSize += data.Flags & MeshFlags_VertexColors ? sizeof(glm::vec4) : 0;
		vertexSize += data.Flags & MeshFlags_Normals ? sizeof(glm::vec3) : 0;
		vertexSize += data.Flags & MeshFlags_Tangent ? sizeof(glm::vec3) : 0;
		vertexSize += data.Flags & MeshFlags_Binormal ? sizeof(glm::vec3) : 0;
		vertexSize += data.Flags & MeshFlags_TextCoord ? sizeof(glm::vec2) : 0;

		return vertexSize;
	}

	MeshData MeshFactory::LoadMeshFromSource(const std::filesystem::path& file)
	{
		HZR_PROFILE_FUNCTION();
		HZR_CORE_ASSERT(File::Exists(file), "File does not exist");

		Timer timer;

		if (m_Handler)
		{
			MeshFactoryProgressHandler* handler = new MeshFactoryProgressHandler(this);
			m_Importer.SetProgressHandler(handler);
		}

		const aiScene* scene = m_Importer.ReadFile(File::GetFileAbsolutePath(file).string(), (uint32_t)m_MeshFlags);
        
		if (!scene || !scene->HasMeshes())
        {
            HZR_CORE_WARN("Scene not found or has no meshes {0}", file.string());
            return MeshData();
        }
		
		m_SceneMeshes = scene->mNumMeshes;
		MeshData data;

		data.SubMeshes.reserve(scene->mNumMeshes);
		ProcessNode(scene->mRootNode, scene, data);

		return data;
	}

	void MeshFactory::ProcessNode(aiNode* node, const aiScene* scene, MeshData& data)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMesh(node, aiMesh, data);
		}
		for (uint32_t i = 0; i < node->mNumChildren; i++)
			ProcessNode(node->mChildren[i], scene, data);
	}

	void MeshFactory::ProcessMesh(aiNode* node, aiMesh* mesh, MeshData& data)
	{
		glm::mat4 transform = AssimpMat4ToGlmMat4(node->mTransformation);
		aiNode* parent = node->mParent;

		while (parent)
		{
			transform *= AssimpMat4ToGlmMat4(parent->mTransformation);
			parent = parent->mParent;
		}

		SubMesh& subMesh = data.SubMeshes.emplace_back();

		subMesh.BaseVertex = data.VertexIndex;
		subMesh.BaseIndex = data.BaseIndex;
		subMesh.MaterialIndex = mesh->mMaterialIndex;

		subMesh.VertexCount = mesh->mNumVertices;
		subMesh.IndexCount = mesh->mNumFaces * 3;

		data.VertexIndex += subMesh.VertexCount;
		data.BaseIndex += subMesh.IndexCount;

		subMesh.MeshName = mesh->mName.C_Str();

		uint32_t color = GetColorChannel(mesh);

		if (mesh->HasPositions())
			data.Flags |= MeshFlags_Positions;
		if (color != UINT32_MAX)
			data.Flags |= MeshFlags_VertexColors;
		if (mesh->HasNormals())
			data.Flags |= MeshFlags_Normals;
		if (mesh->HasTangentsAndBitangents())
			data.Flags |= MeshFlags_Tangent | MeshFlags_Binormal;
		if (mesh->HasTextureCoords(0))
			data.Flags |= MeshFlags_TextCoord;

		//data.Vertices.reserve(mesh->mNumVertices);

		for (uint64_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex3D vertex;

			vertex.Position.x = mesh->mVertices[i].x;
			vertex.Position.y = mesh->mVertices[i].y;
			vertex.Position.z = mesh->mVertices[i].z;
			vertex.Position = m_ScaleMatrix * transform * glm::vec4(vertex.Position, 1.0);

			data.BoundingBox.Encapsulate(vertex.Position);

			if (color != UINT32_MAX)
			{
				aiColor4D c = mesh->mColors[color][i];
				vertex.Color.r = c.r;
				vertex.Color.g = c.g;
				vertex.Color.b = c.b;
				vertex.Color.a = c.a;

			}
			HZR_ASSERT(mesh->HasNormals(), "We need normals");
			{
				vertex.Normals.x = mesh->mNormals[i].x;
				vertex.Normals.y = mesh->mNormals[i].y;
				vertex.Normals.z = mesh->mNormals[i].z;
			}
			if (mesh->HasTangentsAndBitangents())
			{
				vertex.Tangent.x = mesh->mTangents[i].x;
				vertex.Tangent.y = mesh->mTangents[i].y;
				vertex.Tangent.z = mesh->mTangents[i].z;

				vertex.Binormal.x = mesh->mBitangents[i].x;
				vertex.Binormal.y = mesh->mBitangents[i].y;
				vertex.Binormal.z = mesh->mBitangents[i].z;
			}
            
			if (mesh->HasTextureCoords(0))
			{
				vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
				vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
			}
			//data.Vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& face = mesh->mFaces[i];
			//data.Indices.reserve(face.mNumIndices);
			//for (unsigned int j = 0; j < face.mNumIndices; j++)
			//	data.Indices.push_back(face.mIndices[j] + subMesh.BaseVertex);
		}

		m_ProcessedNodes++;

		if (m_Handler)
			m_Handler(0.5f + ((float)m_ProcessedNodes / (float)m_SceneMeshes) * 0.5f);
	}

	void MeshFactory::TraverseNode(aiNode* node, MeshData& data, const glm::mat4 parentTransform, uint32_t level)
	{
		glm::mat4 local = AssimpMat4ToGlmMat4(node->mTransformation);
		glm::mat4 transform = parentTransform * local * m_ScaleMatrix;

		for (uint32_t i = 0; i < node->mNumMeshes; i++) {

			//uint32_t mesh = node->mMeshes[i];
			auto& submesh = data.SubMeshes[i];

			submesh.NodeName = node->mName.C_Str();
			submesh.Transform = transform;
			submesh.LocalTransform = local;
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
			TraverseNode(node->mChildren[i], data, transform, ++level);
	}
}
