
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

		if (!data.GetData())
		{
			HZR_CORE_ERROR("Cannot find asset data for mesh {}", handle);
			return;
		}

		MeshFileHeader header = data.Read<MeshFileHeader>();

		std::vector<Vertex3D> vertices(header.VertexCount);
		std::vector<uint32_t> indices(header.IndexCount);

		for (uint64_t i = 0; i < header.VertexCount; i++)
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

		for (uint64_t i = 0; i < header.IndexCount; i++)
			indices[i] = data.Read<uint32_t>();

		buffer.Release();

		MeshCreateInfo info = {};
		info.DebugName = fmt::format("Mesh {}", handle);
		info.BoundingBox = header.BoundingBox;
		info.VertexCount = vertices.size();
		info.pVertices = vertices.data();
		info.IndexCount = indices.size();
		info.pIndices = indices.data();

		if (info.IndexCount == 0 || info.VertexCount == 0)
			throw JobException("Unable to create mesh");

		Ref<Mesh> asset = Ref<Mesh>::Create(&info);
		asset->IncRefCount();
		job->SetResult(&asset, sizeof(Ref<Mesh>));
	}

	Ref<JobGraph> MeshAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();

		Ref<Job> meshJob = Ref<Job>::Create("Mesh load", LoadMesh, metadata.Handle);
		Ref<JobGraph> graph = Ref<JobGraph>::Create("Mesh", 1);
		graph->GetStage(0)->QueueJobs({ meshJob });

		return graph;
	}

	Ref<JobGraph> MeshAssetLoader::Save(Ref<Asset>& asset)
	{
		return nullptr;
	}

	Ref<JobGraph> MeshAssetLoader::Create(const std::filesystem::path& path)
	{
		HZR_CORE_ASSERT(false, "");
		return Ref<JobGraph>();
	}
}
