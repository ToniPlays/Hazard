#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "Importers/MeshImporter.h"

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
		static void ProcessMeshNode(JobInfo& info, Ref<MeshImporter> importer, uint32_t meshIndex);
		static void FinalizeMesh(JobInfo& info, Ref<MeshImporter> importer);

		static void SaveMesh(JobInfo& info);
	};
}
