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
		static Mesh* LoadMesh(const std::string& file);
		static Mesh* LoadCube();
		static Mesh* LoadSphere();
		static Mesh* LoadPlane();
		static std::vector<Mesh*>& GetLoadedMeshes() { return m_LoadedMeshes; }

	private:
		static void ProcessNode(aiNode* node, const aiScene* scene, MeshData& data);
		static void ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& data);

		static void TraverseNode(aiNode* node, MeshData& data, const glm::mat4 parentTransform = glm::mat4(1.0f), uint32_t level = 0);

		static void GetMaterials(const aiScene* scene, Material& material, const char* path);
		static void LoadMaterialTexture(aiMaterial* material, Material& mat, aiTextureType type, const char* typeName, const char* path);

	private:
		static std::vector<Mesh*> m_LoadedMeshes;
	};
}