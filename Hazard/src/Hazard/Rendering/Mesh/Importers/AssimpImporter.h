#pragma once

#include "MeshImporter.h"
#include "assimp/Importer.hpp"
#include <assimp/scene.h>

namespace Hazard
{
	class AssimpImporter : public MeshImporter
	{
	public:
		AssimpImporter(const std::filesystem::path& file);
		~AssimpImporter() = default;

		MeshImporter::SceneMetadata GetSceneMetadata() override;
		std::vector<MeshImporter::TextureData> GetTextures() override;
		std::vector<MeshImporter::MeshMetadata> GetMeshes() override;
		std::vector<MeshImporter::AnimationData> GetAnimations() override;
		MeshData GetMeshData(uint32_t meshIndex, const std::function<void(uint32_t, uint32_t)>& progress) override;
		glm::mat4 GetMeshTransform(uint32_t meshIndex) override;

	private:
		const aiNode* FindMeshNode(aiNode* root, uint32_t meshIndex);

	private:
		Assimp::Importer m_Importer;
		std::filesystem::path m_SourcePath;
	};
}