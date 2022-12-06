#pragma once

#include "MeshCreateInfo.h"
#include "Mesh.h"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace Hazard
{
	enum MeshFlags : uint32_t
	{
		MeshFlags_Positions = BIT(0),
		MeshFlags_VertexColors = BIT(1),
		MeshFlags_Normals = BIT(2),
		MeshFlags_Tangent = BIT(3),
		MeshFlags_Binormal = BIT(4),
		MeshFlags_TextCoord = BIT(5)
	};

	class MeshFactory 
	{
	public:
		MeshFactory()
		{
			m_ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		}
		~MeshFactory() = default;

		static void SetCacheLocation(const std::filesystem::path& path) { s_CacheDirectory = path; }

		void SetOptimization(MeshLoaderFlags flags);
		void SetScalar(float scalar)
		{
			m_ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scalar));
		}


		MeshData LoadMeshFromSource(const std::filesystem::path& file);
		CacheStatus CacheStatus(const AssetHandle& handle);
		std::filesystem::path GetCacheFile(const AssetHandle& handle);
		size_t GetMeshDataSize(const MeshData& data);

	private:
		uint32_t GetColorChannel(const aiMesh* mesh);

		void ProcessNode(aiNode* node, const aiScene* scene, MeshData& data);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& data);
		void TraverseNode(aiNode* node, MeshData& data, const glm::mat4 parentTransform = glm::mat4(1.0f), uint32_t level = 0);

	private:
		MeshLoaderFlags m_MeshFlags;
		glm::mat4 m_ScaleMatrix = glm::mat4(1.0f);

		inline static std::filesystem::path s_CacheDirectory = "Library/Mesh/";
	};
}