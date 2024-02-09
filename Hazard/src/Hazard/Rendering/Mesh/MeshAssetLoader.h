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
		Ref<JobGraph> Save(Ref<Asset> asset, const SaveAssetSettings& settings) override;
		Ref<JobGraph> Create(const CreateAssetSettings& settings) override;
	};
}
