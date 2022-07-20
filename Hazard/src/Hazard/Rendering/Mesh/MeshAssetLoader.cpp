
#include <hzrpch.h>
#include "MeshAssetLoader.h"
#include "MeshFactory.h"

namespace Hazard {
	bool MeshAssetLoader::Load(AssetMetadata& path, Ref<Asset>& asset)
	{
		MeshFactory factory;

		factory.SetOptimization(MeshFlags_DefaultOptimization);
		asset = factory.LoadMesh(path.Path);
		return asset;
	}
	bool MeshAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}