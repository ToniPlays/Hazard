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
		};

	public:
		MeshAssetLoader() = default;
		~MeshAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
		Ref<JobGraph> Create(const CreateAssetSettings& settings) override;

	private:
		static void PreprocessDependencies(JobInfo& info, Ref<MeshImporter> importer, const CreateSettings& settings);
		static void ProcessMeshNode(JobInfo& info, Ref<MeshImporter> importer, const MeshImporter::MeshMetadata& mesh);
		static void ProcessMaterial(JobInfo& info, Ref<MeshImporter> importer, const MeshImporter::MaterialMetadata& material, const std::filesystem::path& maetrialRoot);
		static void FinalizeMesh(JobInfo& info, Ref<MeshImporter> importer);

		static void ReadMeshDataFromGPU(JobInfo& info, Ref<Mesh> mesh);
		static void CompileMesh(JobInfo& info, Ref<Mesh> mesh);

		static void CreateMeshFromSource(JobInfo& info, AssetHandle handle);

		static void SetMaterialProperties(Ref<Material> material, const MeshImporter::MaterialData& materialData);
	};
}
