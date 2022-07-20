#pragma once

#include "MeshCreateInfo.h"
#include "Mesh.h"

namespace Hazard
{
	class MeshFactory {
	public:
		MeshFactory() = default;
		~MeshFactory() = default;

		void SetOptimization(uint32_t flags);
		Ref<Mesh> LoadMesh(const std::filesystem::path& file);
	private:
		static void ProcessNode(aiNode* node, const aiScene* scene, MeshData& data);
		static void ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& data);
		static void TraverseNode(aiNode* node, MeshData& data, const glm::mat4 parentTransform = glm::mat4(1.0f), uint32_t level = 0);

	private:
		uint32_t m_MeshFlags = 0;
	};
}