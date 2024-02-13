#pragma once

#include "MeshImporter.h"
#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <assimp/ProgressHandler.hpp>
#include "MathCore.h"

#include "Callback.h"

namespace Hazard
{
	class AssimpImporter : public MeshImporter
	{
	public:
		AssimpImporter(const std::filesystem::path& file);
		~AssimpImporter() = default;

		MeshImporter::SceneMetadata GetSceneMetadata() override;
		std::vector<MeshImporter::TextureMetadata> GetTextures() override;
		std::vector<MaterialMetadata> GetMaterials() override;
		std::vector<MeshImporter::MeshMetadata> GetMeshes() override;
		std::vector<MeshImporter::AnimationData> GetAnimations() override;
		MeshData GetMeshData(const MeshMetadata& mesh, const std::function<void(uint32_t, uint32_t)>& progress) override;
		glm::mat4 GetMeshTransform(uint32_t meshIndex) override;

		void AddImportProgressCallback(const std::function<void(float)> callback) override
		{
			m_LoadCallback.Add(callback);
		}

	private:

		aiMatrix4x4 GetMeshAiTransform(const aiNode* node);
		const aiNode* FindMeshNode(aiNode* root, uint32_t meshIndex);
		const aiScene* GetScene();
		MeshImporter::MeshMetadata GetMeshDataFromNode(aiNode* node);

		void PrintHierarchy(const aiNode* node, uint32_t level);
		std::unordered_map<TextureType, uint32_t> GetMaterialTextures(aiMaterial* material);

	private:
		Assimp::Importer m_Importer;
		std::filesystem::path m_SourcePath;

		Callback<void(float)> m_LoadCallback;
	};

	class AssimpProgressHandler : public Assimp::ProgressHandler
	{
	public:
		AssimpProgressHandler(const std::function<void(float)>& func) : m_Func(func) {}

		bool Update(float progress) override
		{
			if (m_Func)
				m_Func(Math::Clamp(progress * 1.0f, 0.0f, 1.0f));
			return true;
		}
	private:
		std::function<void(float)> m_Func;
	};
}