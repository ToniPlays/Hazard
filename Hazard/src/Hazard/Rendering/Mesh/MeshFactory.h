#pragma once

#include "MeshCreateInfo.h"
#include "Mesh.h"

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
	//Used for saving a mesh
	struct MeshCacheData
	{
		//Bounding box
		//Metadata
		uint32_t Flags;
		size_t VertexCount;
		size_t IndexCount;
	};

	class MeshFactory 
	{
	public:
		MeshFactory() = default;
		~MeshFactory() = default;

		void SetOptimization(MeshLoaderFlags flags);
		MeshData LoadMeshFromSource(const std::filesystem::path& file);
		MeshData LoadMeshFromCache(const AssetHandle& handle);
		CacheStatus CacheStatus(const AssetHandle& handle);
		void SaveMeshToCache(const AssetHandle& handle, const MeshData& data);

	private:
		std::filesystem::path GetCacheFile(const AssetHandle& handle);

		uint32_t GetColorChannel(const aiMesh* mesh);
		size_t GetMeshDataSize(const MeshData& data);

		void ProcessNode(aiNode* node, const aiScene* scene, MeshData& data);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& data);
		void TraverseNode(aiNode* node, MeshData& data, const glm::mat4 parentTransform = glm::mat4(1.0f), uint32_t level = 0);

	private:
		MeshLoaderFlags m_MeshFlags;

		inline static std::filesystem::path s_CacheDirectory = "Library/Mesh/";
	};
}