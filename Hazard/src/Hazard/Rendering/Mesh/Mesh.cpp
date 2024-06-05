#include <hzrpch.h>

#include "Mesh.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "HazardRendererCore.h"

namespace Hazard
{
	void Mesh::GenerateMesh(const std::vector<MeshImporter::MeshData>& meshData)
	{
		HZR_PROFILE_FUNCTION();
		uint64_t vertexOffset = 0;
		uint64_t indexOffset = 0;

		for (auto& submesh : meshData)
		{
			if (submesh.Vertices.size() == 0) continue;

			SubmeshData data = {
				.NodeName = submesh.Name,
				.Transform = submesh.Transform,
				.NodeID = UID(),
				.VertexCount = submesh.Vertices.size(),
				.IndexCount = submesh.Indices.size(),
				.VertexOffset = vertexOffset,
				.IndexOffset = indexOffset,
			};

			Buffer vertices = Buffer((uint8_t*)submesh.Vertices.data(), submesh.Vertices.size() * sizeof(Vertex3D));
			Buffer indices = Buffer((uint8_t*)submesh.Indices.data(), submesh.Indices.size() * sizeof(uint32_t));

			CreateSubmeshResources(data, vertices, indices);

			m_SubmeshData[data.NodeID] = data;

			vertexOffset += vertices.Size;
			indexOffset += indices.Size;
		}
	}

	void Mesh::GenerateMesh(const std::vector<SubmeshData>& submeshes, Buffer vertexData, Buffer indexData)
	{
		for (auto& submesh : submeshes)
		{
			Buffer vertices = Buffer((uint8_t*)vertexData.Data + submesh.VertexOffset, submesh.VertexCount * sizeof(Vertex3D));
			Buffer indices = Buffer((uint8_t*)indexData.Data + submesh.IndexOffset, submesh.IndexCount * sizeof(uint32_t));

			CreateSubmeshResources(submesh, vertices, indices);

			m_SubmeshData[submesh.NodeID] = submesh;
		}
	}

	void Mesh::SetSubmeshMaterialHandle(uint64_t node, AssetHandle handle)
	{
		m_SubmeshData[node].DefaultMaterialHandle = handle;
	}

	uint64_t Mesh::GetSubmeshNodeFromName(const std::string& name)
	{
		for (auto& [id, data] : m_SubmeshData)
			if (data.NodeName == name) return data.NodeID;

		return 0;
	}

	uint64_t Mesh::CalculateTotalVertexCount()
	{
		HZR_PROFILE_FUNCTION();

		uint64_t count = 0;
		for (auto& [node, mesh] : m_SubmeshData)
			count += mesh.VertexCount;

		return count;
	}

	uint64_t Mesh::CalculateTotalIndexCount()
	{
		HZR_PROFILE_FUNCTION();

		uint64_t count = 0;
		for (auto& [node, mesh] : m_SubmeshData)
			count += mesh.IndexCount;

		return count;
	}
	void Mesh::CreateSubmeshResources(const SubmeshData& submesh, Buffer vertices, Buffer indices)
	{
		uint64_t handle = submesh.NodeID;

		BufferCreateInfo vertexBufferInfo = {
			.Name = fmt::format("VertexBuffer {} {}", File::GetName(m_SourceAssetPath), submesh.NodeName),
			.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_SRC,
			.Size = vertices.Size,
			.Data = vertices.Data,
		};

		HZR_ASSERT(!m_VertexBuffers.contains(handle), "Something is wrong I can feel it");
		m_VertexBuffers[handle] = GPUBuffer::Create(&vertexBufferInfo);

		BufferCreateInfo indexBufferInfo = {
			.Name = fmt::format("IndexBuffer {} {}", File::GetName(m_SourceAssetPath), submesh.NodeName),
			.UsageFlags = BUFFER_USAGE_INDEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_SRC,
			.Size = indices.Size,
			.Data = indices.Data,
		};
		HZR_ASSERT(!m_IndexBuffers.contains(handle), "Something is wrong I can feel it");
		m_IndexBuffers[handle] = GPUBuffer::Create(&indexBufferInfo);
	}
}

