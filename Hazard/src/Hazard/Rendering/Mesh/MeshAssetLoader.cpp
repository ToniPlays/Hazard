
#include <hzrpch.h>

#include "MeshAssetLoader.h"
#include "MeshFactory.h"
#include "Hazard/Assets/AssetPack.h"
#include "Hazard/Assets/AssetManager.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{
	static void LoadMesh(Ref<Job> job, AssetHandle handle)
	{
		Buffer buffer = AssetManager::GetAssetData(handle);
		CachedBuffer data(buffer.Data, buffer.Size);
		if (!data.GetData()) return;

		MeshFileHeader header = data.Read<MeshFileHeader>();

		std::vector<Vertex3D> vertices(header.VertexCount);
		std::vector<uint32_t> indices(header.IndexCount);

		for (size_t i = 0; i < header.VertexCount; i++)
		{
			Vertex3D& v = vertices[i];
			if (header.Flags & MeshFlags_Positions)
				v.Position = data.Read<glm::vec3>();
			if (header.Flags & MeshFlags_VertexColors)
				v.Color = data.Read<glm::vec4>();
			if (header.Flags & MeshFlags_Normals)
				v.Normals = data.Read<glm::vec3>();
			if (header.Flags & MeshFlags_Tangent)
				v.Tangent = data.Read<glm::vec3>();
			if (header.Flags & MeshFlags_Binormal)
				v.Binormal = data.Read<glm::vec3>();
			if (header.Flags & MeshFlags_TextCoord)
				v.TexCoords = data.Read<glm::vec2>();
		}
		for (size_t i = 0; i < header.IndexCount; i++)
			indices[i] = data.Read<uint32_t>();

		MeshCreateInfo info = {};
		info.DebugName = fmt::format("Mesh {}", handle);
		info.BoundingBox = header.BoundingBox;
		info.UsageFlags = 0;
		info.VertexCount = vertices.size() * sizeof(Vertex3D);
		info.pVertices = vertices.data();
		info.IndexCount = indices.size() * sizeof(uint32_t);
		info.pIndices = indices.data();

		Ref<Mesh> asset = Ref<Mesh>::Create(&info);
		asset->IncRefCount();
		job->GetStage()->SetResult(asset);

		AssetManager::GetMetadata(handle).LoadState = LoadState::Loaded;

		buffer.Release();
	}

	Ref<JobGraph> MeshAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();

		Ref<Job> meshJob = Ref<Job>::Create(LoadMesh, metadata.Handle);
		Ref<JobGraph> graph = Ref<JobGraph>::Create("Mesh", 1);
		graph->GetStage(0)->QueueJobs({ meshJob });

		return graph;
	}
	Ref<JobGraph> MeshAssetLoader::Save(Ref<Asset>& asset)
	{
		return nullptr;
	}
}
