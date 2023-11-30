#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "BoundingBox.h"
#include "Mesh.h"

namespace Hazard 
{
	struct MeshFileHeader
	{
		//Metadata
		uint32_t Flags;
		uint64_t VertexCount;
		uint64_t IndexCount;
		BoundingBox BoundingBox;
	};

	class MeshAssetLoader : public IAssetLoader 
	{
	public:
		MeshAssetLoader() = default;
		~MeshAssetLoader() = default;

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset>& asset) override;
		Ref<JobGraph> DataFromSource(const std::filesystem::path& path) override;
		Ref<JobGraph> Create(const std::filesystem::path& path, const std::filesystem::path& internalPath) override;

		Buffer ToBinary(Ref<Asset> asset) override { return Buffer(); };

	private:
#if defined HZR_PLATFORM_WINDOWS || defined HZR_PLATFORM_MACOS
		static void LoadMeshFromSource(JobInfo& info, const std::filesystem::path& path);
		static void ProcessSubmesh(JobInfo& info, const SubMesh& mesh);
		static void FinalizeMesh(JobInfo& info, const std::filesystem::path& path);
#endif
	};
}
