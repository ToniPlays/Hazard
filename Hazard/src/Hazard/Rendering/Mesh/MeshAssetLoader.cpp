
#include <hzrpch.h>

#include "MeshAssetLoader.h"
#include "MeshFactory.h"
#include "Hazard/Assets/AssetPack.h"
#include "Hazard/Assets/AssetManager.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{
	LoadType MeshAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags)
	{
		HZR_PROFILE_FUNCTION();
		Timer timer;
		MeshFactory factory = {};

		factory.SetOptimization(MeshLoaderFlags_DefaultFlags);
		CacheStatus status = factory.CacheStatus(metadata.Handle);

		if (status == CacheStatus::Exists)
		{
			MeshData result;
			CachedBuffer buffer = File::ReadBinaryFile(factory.GetCacheFile(metadata.Handle));

			buffer.Read<AssetPackElement>();
			MeshCacheData data = buffer.Read<MeshCacheData>();
			result.BoundingBox = data.BoundingBox;

			result.Vertices.resize(data.VertexCount);
			result.Indices.resize(data.IndexCount);

			for (size_t i = 0; i < data.VertexCount; i++)
			{
				Vertex3D& v = result.Vertices[i];
				if (data.Flags & MeshFlags_Positions)
					v.Position = { buffer.Read<glm::vec3>(), 1.0 };
				if (data.Flags & MeshFlags_VertexColors)
					v.Color = buffer.Read<glm::vec4>();
				if (data.Flags & MeshFlags_Normals)
					v.Normals = { buffer.Read<glm::vec3>(), 1.0 };
				if (data.Flags & MeshFlags_Tangent)
					v.Tangent = { buffer.Read<glm::vec3>(), 1.0 };
				if (data.Flags & MeshFlags_Binormal)
					v.Binormal = { buffer.Read<glm::vec3>(), 1.0 };
				if (data.Flags & MeshFlags_TextCoord)
					v.TexCoords = buffer.Read<glm::vec2>();
			}
			for (size_t i = 0; i < data.IndexCount; i++)
				result.Indices[i] = buffer.Read<uint32_t>();

			MeshCreateInfo meshInfo = {};
            meshInfo.DebugName = metadata.Path.filename();
			meshInfo.Usage = HazardRenderer::BufferUsage::StaticDraw;
			meshInfo.BoundingBox = result.BoundingBox;
			meshInfo.VertexCount = result.Vertices.size() * sizeof(Vertex3D);
			meshInfo.pVertices = result.Vertices.data();
			meshInfo.IndexCount = result.Indices.size() * sizeof(uint32_t);
			meshInfo.pIndices = result.Indices.data();

			asset = Ref<Mesh>::Create(&meshInfo);
			HZR_CORE_INFO("Mesh asset loaded in {0}", timer.ElapsedMillis());
			return LoadType::Cache;
		}
		if (!File::Exists(metadata.Path))
		{
			HZR_CORE_INFO("Mesh asset failed to load in {0}", timer.ElapsedMillis());
			return LoadType::Failed;
		}

		MeshData meshData = factory.LoadMeshFromSource(metadata.Path);

		MeshCreateInfo meshInfo = {};
        meshInfo.DebugName = metadata.Path.filename();
		meshInfo.Usage = HazardRenderer::BufferUsage::StaticDraw;
		meshInfo.BoundingBox = meshData.BoundingBox;
		meshInfo.VertexCount = meshData.Vertices.size() * sizeof(Vertex3D);
		meshInfo.pVertices = meshData.Vertices.data();
		meshInfo.IndexCount = meshData.Indices.size() * sizeof(uint32_t);
		meshInfo.pIndices = meshData.Indices.data();

		asset = Ref<Mesh>::Create(&meshInfo);

		HZR_CORE_INFO("Mesh asset loaded in {0}", timer.ElapsedMillis());
		return LoadType::Source;
	}
	Ref<JobGraph> MeshAssetLoader::LoadAsync(AssetMetadata& metadata, uint32_t flags)
	{
		Ref<MeshFactory> factory = Ref<MeshFactory>::Create();
		factory->SetOptimization(MeshLoaderFlags_DefaultFlags);
		Ref<JobGraph> graph = factory->LoadMeshFromSourceAsync(metadata.Path);
		return graph;
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

		size_t dataSize = factory.GetMeshDataSize(data);
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
				buffer.Write<glm::vec3>({ v.Position.x, v.Position.y, v.Position.z });
			if (data.Flags & MeshFlags_VertexColors)
				buffer.Write(v.Color);
			if (data.Flags & MeshFlags_Normals)
				buffer.Write<glm::vec3>({ v.Normals.x, v.Normals.y, v.Normals.z });
			if (data.Flags & MeshFlags_Tangent)
				buffer.Write<glm::vec3>({ v.Tangent.x, v.Tangent.y, v.Tangent.z });
			if (data.Flags & MeshFlags_Binormal)
				buffer.Write<glm::vec3>({ v.Binormal.x, v.Binormal.y, v.Binormal.z });
			if (data.Flags & MeshFlags_TextCoord)
				buffer.Write(v.TexCoords);
		}
		buffer.Write(data.Indices.data(), data.Indices.size() * sizeof(uint32_t));

		auto path = factory.GetCacheFile(metadata.Handle);

		if (!File::DirectoryExists(path.parent_path()))
			File::CreateDir(path.parent_path());

		File::WriteBinaryFile(path, buffer.GetData(), buffer.GetSize());
		return false;
	}
	Ref<JobGraph> MeshAssetLoader::SaveAsync(Ref<Asset>& asset)
	{
		HZR_CORE_ASSERT(false, "TODO");
		return nullptr;
	}
}
