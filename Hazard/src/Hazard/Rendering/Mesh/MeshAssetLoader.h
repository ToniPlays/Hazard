#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "BoundingBox.h"

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
		Ref<JobGraph> FromSourceFile(const std::filesystem::path& path) override { return nullptr; };
		Ref<JobGraph> Create(const std::filesystem::path& path) override;

		Buffer ToBinary(Ref<Asset> asset) override { return Buffer(); };
	};
}