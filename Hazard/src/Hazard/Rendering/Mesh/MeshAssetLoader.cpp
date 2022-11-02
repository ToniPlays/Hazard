
#include <hzrpch.h>
#include "MeshAssetLoader.h"
#include "MeshFactory.h"

namespace Hazard
{
	LoadType MeshAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
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
		CacheStatus status = factory.CacheStatus(metadata.Handle);
		if (status == CacheStatus::Exists)
		{
			MeshData meshData = factory.LoadMeshFromCache(metadata.Handle);
			asset = Ref<Mesh>::Create(meshData, meshData.Vertices, meshData.Indices);
			return LoadType::Cache;
		}
		if (!File::Exists(metadata.Path))
			return LoadType::Failed;

		MeshData meshData = factory.LoadMeshFromSource(metadata.Path);
		asset = Ref<Mesh>::Create(meshData, meshData.Vertices, meshData.Indices);
		return LoadType::Source;
	}
	bool MeshAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}