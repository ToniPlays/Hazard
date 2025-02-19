#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "Importers/MeshImporter.h"
#include "Mesh.h"

namespace Hazard 
{

	class Material;

	enum MeshCreateFlags
	{
		MESH_CREATE_INCLUDE_CAMERA = BIT(0),
		MESH_CREATE_INCLUDE_ANIMATIONS = BIT(1),
		MESH_CREATE_INCLUDE_LIGHTS = BIT(2),
		MESH_CREATE_INCLUDE_TEXTURES = BIT(3),
		MESH_CREATE_INCLUDE_MATERIALS = BIT(4),
	};

	struct MeshFileHeader
	{
		uint64_t SubmeshCount;
		uint64_t VertexCount;
		uint64_t IndexCount;
	};

	struct SubmeshHeader
	{
		UID NodeID;
		glm::mat4 Transform;
		uint64_t MaterialHandle;
		uint64_t VertexCount;
		uint64_t IndexCount;
		uint64_t VertexOffset;
		uint64_t IndexOffset;
		//Variable name
	};

	class MeshAssetLoader : public IAssetLoader 
	{
	public:

		struct CreateSettings
		{
			uint32_t Flags = 0;
			std::filesystem::path MaterialPath;
			std::filesystem::path TexturePath;
		};

	public:
		MeshAssetLoader() = default;
		~MeshAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata, const LoadAssetSettings& settings) override;
		Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
		Ref<JobGraph> Create(const CreateAssetSettings& settings) override;

	private:
		static Coroutine CreateMeshFromSource(JobInfo info, AssetMetadata& metadata);

		static Coroutine PreprocessDependencies(JobInfo info, Ref<MeshImporter> importer, const CreateSettings& settings);
		static Coroutine ProcessMeshNode(JobInfo info, Ref<MeshImporter> importer, const MeshImporter::MeshMetadata& mesh);
		static Coroutine ProcessMaterial(JobInfo info, Ref<MeshImporter> importer, const MeshImporter::MaterialMetadata& material, const std::filesystem::path& materialRoot);
		static Coroutine ProcessTexture(JobInfo info, Ref<MeshImporter> importer, const MeshImporter::TextureMetadata& texture, const std::filesystem::path& textureRoot);
		static Coroutine FinalizeMesh(JobInfo info, Ref<MeshImporter> importer, const CreateSettings& settings);

		static Coroutine ReadMeshDataFromGPU(JobInfo info, Ref<Mesh> mesh);
		static Coroutine CompileMesh(JobInfo info, Ref<Mesh> mesh);


		static void SetMaterialProperties(Ref<Material> material, const MeshImporter::MaterialData& materialData);
		static void SetMaterialTextures(Ref<Material> material, std::unordered_map<MeshImporter::TextureType, MeshImporter::TextureMetadata>& textures, const std::unordered_map<std::string, AssetHandle> assets);
	};
}
