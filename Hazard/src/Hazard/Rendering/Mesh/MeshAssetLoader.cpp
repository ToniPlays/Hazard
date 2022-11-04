
#include <hzrpch.h>
#include "MeshAssetLoader.h"
#include "MeshFactory.h"
#include "Hazard/Assets/AssetPack.h"
#include "Hazard/Assets/AssetManager.h"

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
			MeshData result;
			CachedBuffer buffer = File::ReadBinaryFile(factory.GetCacheFile(metadata.Handle));

			AssetPackElement element = buffer.Read<AssetPackElement>();
			MeshCacheData data = buffer.Read<MeshCacheData>();
			result.BoundingBox = data.BoundingBox;

			result.Vertices.resize(data.VertexCount);
			result.Indices.resize(data.IndexCount);

			for (size_t i = 0; i < data.VertexCount; i++)
			{
				Vertex3D& v = result.Vertices[i];
				if (data.Flags & MeshFlags_Positions)
				{
					v.Position = buffer.Read<glm::vec3>();
				}
				if (data.Flags & MeshFlags_VertexColors)
					v.Color = buffer.Read<glm::vec4>();
				if (data.Flags & MeshFlags_Normals)
					v.Normals = buffer.Read<glm::vec3>();
				if (data.Flags & MeshFlags_Tangent)
					v.Tangent = buffer.Read<glm::vec3>();
				if (data.Flags & MeshFlags_Binormal)
					v.Binormal = buffer.Read<glm::vec3>();
				if (data.Flags & MeshFlags_TextCoord)
					v.TexCoords = buffer.Read<glm::vec2>();
			}
			for (size_t i = 0; i < data.IndexCount; i++)
				result.Indices[i] = buffer.Read<uint32_t>();

			asset = Ref<Mesh>::Create(result, result.Vertices, result.Indices);
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
		AssetMetadata& metadata = AssetManager::GetMetadata(asset->GetHandle());
		
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

		MeshData data = factory.LoadMeshFromSource(metadata.Path);

		uint32_t dataSize = factory.GetMeshDataSize(data);
		CachedBuffer buffer(sizeof(AssetPackElement) + sizeof(MeshCacheData) + dataSize);

		AssetPackElement element = {};
		element.Type = (uint32_t)asset->GetType();
		element.Handle = asset->GetHandle();
		element.AssetDataSize = dataSize;

		MeshCacheData meshData = {};
		meshData.Flags = data.Flags;
		meshData.VertexCount = data.Vertices.size();
		meshData.IndexCount = data.Indices.size();
		meshData.BoundingBox = data.BoundingBox;

		buffer.Write(element);
		buffer.Write(meshData);
		
		for (auto& v : data.Vertices)
		{
			if (data.Flags & MeshFlags_Positions)
				buffer.Write(v.Position);
			if (data.Flags & MeshFlags_VertexColors)
				buffer.Write(v.Color);
			if (data.Flags & MeshFlags_Normals)
				buffer.Write(v.Normals);
			if (data.Flags & MeshFlags_Tangent)
				buffer.Write(v.Tangent);
			if (data.Flags & MeshFlags_Binormal)
				buffer.Write(v.Binormal);
			if (data.Flags & MeshFlags_TextCoord)
				buffer.Write(v.TexCoords);
		}
		buffer.Write(data.Indices.data(), data.Indices.size() * sizeof(uint32_t));

		auto& path = factory.GetCacheFile(metadata.Handle);

		if (!File::DirectoryExists(path.parent_path()))
			File::CreateDir(path.parent_path());

		File::WriteBinaryFile(path, buffer.GetData(), buffer.GetSize());
		return false;
	}
}