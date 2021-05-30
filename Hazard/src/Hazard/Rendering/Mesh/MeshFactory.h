#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>

namespace Hazard::Rendering {

	struct MeshData {
		uint32_t vertexIndex = 0, baseIndex = 0;
		std::vector<SubMesh> subMeshes;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};

	class MeshFactory {

	public:
		static Ref<Mesh> LoadMesh(const std::string& file);
		static Ref<Mesh> LoadCube();
		static Ref<Mesh> LoadSphere();
		static Ref<Mesh> LoadPlane();
		static std::vector<Mesh*>& GetLoadedMeshes() { return m_LoadedMeshes; }

	private:
		static void ProcessNode(aiNode* node, const aiScene* scene, MeshData& data);
		static void ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& data);

		static void TraverseNode(aiNode* node, MeshData& data, const glm::mat4 parentTransform = glm::mat4(1.0f), uint32_t level = 0);
	private:
		static std::vector<Mesh*> m_LoadedMeshes;
	};
}