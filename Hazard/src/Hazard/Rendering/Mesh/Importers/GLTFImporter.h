#pragma once

#include "MeshImporter.h"

namespace Hazard
{
	class GLTFImporter : public MeshImporter
	{
	public:
		GLTFImporter(const std::filesystem::path& file);
		~GLTFImporter() = default;

		MeshImporter::SceneMetadata GetSceneMetadata() override;
		std::vector<MeshImporter::TextureData> GetTextures() override;
		std::vector<MeshImporter::MeshMetadata> GetMeshes() override;
		std::vector<MeshImporter::AnimationData> GetAnimations() override;
		MeshImporter::MeshData GetMeshData(const MeshMetadata& mesh, const std::function<void(uint32_t, uint32_t)>& progress) override;
		glm::mat4 GetMeshTransform(uint32_t meshIndex) override;

		void AddImportProgressCallback(const std::function<void(float)> callback) override {};

	private:
		std::filesystem::path m_SourcePath;
	};
}