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
		static Mesh* LoadMesh(const char* file);
		static Mesh* LoadCube();
		static std::unordered_map<std::string, Mesh*>& GetLoadedMeshes() { return loadedMeshes; }

	private:
		static void ProcessNode(aiNode* node, const aiScene* scene, MeshData& data);
		static void ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& data);

		static void GetMaterials(const aiScene* scene, Material& material);
		static void LoadMaterialTexture(aiMaterial* material, Material& mat, aiTextureType type, const char* typeName);

	private:
		static std::unordered_map<std::string, Mesh*> loadedMeshes;
	};
}