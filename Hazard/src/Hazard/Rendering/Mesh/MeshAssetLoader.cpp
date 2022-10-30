
#include <hzrpch.h>
#include "MeshAssetLoader.h"
#include "MeshFactory.h"

namespace Hazard 
{
	bool MeshAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		MeshFactory factory = {};

		uint32_t flags = MeshLoaderFlags_CalculateTangentSpace;
		flags |= MeshLoaderFlags_Triangulate;
		flags |= MeshLoaderFlags_SortByType;
		flags |= MeshLoaderFlags_GenerateNormals;
		flags |= MeshLoaderFlags_GenerateUVCoords;
		flags |= MeshLoaderFlags_OptimizeMeshes;
		flags |= MeshLoaderFlags_JoinIdenticalVertices;
		flags |= MeshLoaderFlags_ValidateDataStructure;

		factory.SetOptimization((MeshLoaderFlags)flags);
		MeshData meshData = factory.LoadMeshFromCacheOrReload(metadata.Handle, metadata.Path);
		asset = Ref<Mesh>::Create(meshData, meshData.Vertices, meshData.Indices);
		return asset;
	}
	bool MeshAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}