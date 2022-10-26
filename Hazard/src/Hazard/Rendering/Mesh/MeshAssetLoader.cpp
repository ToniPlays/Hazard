
#include <hzrpch.h>
#include "MeshAssetLoader.h"
#include "MeshFactory.h"

namespace Hazard {
	bool MeshAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		MeshFactory factory = {};

		uint32_t flags = MeshFlags_CalculateTangentSpace;
		flags |= MeshFlags_Triangulate;
		flags |= MeshFlags_SortByType;
		flags |= MeshFlags_GenerateNormals;
		flags |= MeshFlags_GenerateUVCoords;
		flags |= MeshFlags_OptimizeMeshes;
		flags |= MeshFlags_JoinIdenticalVertices;
		flags |= MeshFlags_ValidateDataStructure;

		factory.SetOptimization((MeshFlags)flags);
		asset = factory.LoadMesh(metadata.Path);
		return asset;
	}
	bool MeshAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}