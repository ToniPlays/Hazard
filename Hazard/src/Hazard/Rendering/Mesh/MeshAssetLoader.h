#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "Importers/MeshImporter.h"
#include "Mesh.h"

namespace Hazard 
{
	enum MeshCreateFlags
	{
		MESH_CREATE_INCLUDE_CAMERA = BIT(0),
		MESH_CREATE_INCLUDE_ANIMATIONS = BIT(1),
		MESH_CREATE_INCLUDE_LIGHTS = BIT(2),
		MESH_CREATE_INCLUDE_TEXTURES = BIT(3),
		MESH_CREATE_INCLUDE_MATERIALS = BIT(4),
	};

	struct MeshCreationSettings
	{
		uint32_t Flags;
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
		uint32_t VertexCount;
		uint32_t IndexCount;
		uint32_t VertexOffset;
		uint32_t IndexOffset;
		//Variable name
	};

	class MeshAssetLoader : public IAssetLoader 
	{
	public:
		MeshAssetLoader() = default;
		~MeshAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
		Ref<JobGraph> Create(const CreateAssetSettings& settings) override;

	private:
		static void PreprocessDependencies(JobInfo& info, Ref<MeshImporter> importer);
		static void ProcessMeshNode(JobInfo& info, Ref<MeshImporter> importer, const MeshImporter::MeshMetadata& mesh);
		static void FinalizeMesh(JobInfo& info, Ref<MeshImporter> importer);

		static void ReadMeshDataFromGPU(JobInfo& info, Ref<Mesh> mesh);
		static void CompileMesh(JobInfo& info, Ref<Mesh> mesh);

		static void CreateMeshFromSource(JobInfo& info, AssetHandle handle);
	};
}
