#pragma once

#include "MeshCreateInfo.h"
#include "Mesh.h"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/ProgressHandler.hpp>

namespace Hazard
{
	using MeshProgressCallback = std::function<void(float)>;

	enum MeshFlags : uint32_t
	{
		MeshFlags_Positions = BIT(0),
		MeshFlags_VertexColors = BIT(1),
		MeshFlags_Normals = BIT(2),
		MeshFlags_Tangent = BIT(3),
		MeshFlags_Binormal = BIT(4),
		MeshFlags_TextCoord = BIT(5)
	};

	class MeshFactory : public RefCount
	{
		friend class MeshFactoryProgressHandler;
	public:
		MeshFactory()
		{
			m_ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		}
		~MeshFactory() = default;

		void SetOptimization(MeshLoaderFlags flags);
		void SetScalar(float scalar)
		{
			m_ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scalar));
		}

		void SetProgressHandler(MeshProgressCallback handler) { m_Handler = handler; }

		MeshData LoadMeshFromSource(const std::filesystem::path& file);
		size_t GetMeshDataSize(const MeshData& data);
		size_t GetVertexSize(const MeshData& data);

	private:
		static uint32_t GetColorChannel(const aiMesh* mesh);

		void ProcessNode(aiNode* node, const aiScene* scene, MeshData& data);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& data);
		void TraverseNode(aiNode* node, MeshData& data, const glm::mat4 parentTransform = glm::mat4(1.0f), uint32_t level = 0);

	private:
		MeshLoaderFlags m_MeshFlags;
		glm::mat4 m_ScaleMatrix = glm::mat4(1.0f);
		MeshProgressCallback m_Handler;

		std::atomic_size_t m_SceneMeshes = 0;
		std::atomic_size_t m_ProcessedNodes = 0;

		Assimp::Importer m_Importer;
	};

	class MeshFactoryProgressHandler : public Assimp::ProgressHandler
	{
	public:
		MeshFactoryProgressHandler(MeshFactory* factory) : m_Factory(factory) {}

		bool Update(float progress) override
		{
			if(m_Factory->m_Handler)
				m_Factory->m_Handler(progress * 0.5f);
			return true;
		}
	private:
		MeshFactory* m_Factory;
	};
}