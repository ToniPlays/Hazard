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

		Ref<JobGraph> Load(AssetMetadata& metadata) override;
		Ref<JobGraph> Save(Ref<Asset>& asset) override;
	};
}