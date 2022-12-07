#pragma once

#include "Hazard/Assets/IAssetLoader.h"
#include "BoundingBox.h"

namespace Hazard 
{
	struct MeshCacheData
	{
		//Metadata
		uint32_t Flags;
		size_t VertexCount;
		size_t IndexCount;
		BoundingBox BoundingBox;
	};

	class MeshAssetLoader : public IAssetLoader 
	{
	public:
		MeshAssetLoader() = default;
		~MeshAssetLoader() = default;

		LoadType Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags = 0) override;
		Ref<JobGraph> LoadAsync(AssetMetadata& metadata, uint32_t flags = 0) override;
		bool Save(Ref<Asset>& asset) override;
		Ref<JobGraph> SaveAsync(Ref<Asset>& asset) override;
	};
}