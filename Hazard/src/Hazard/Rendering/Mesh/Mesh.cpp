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
				.NodeID = UID(),
				.VertexCount = submesh.Vertices.size(),
				.IndexCount = submesh.Indices.size(),
				.VertexOffset = vertexOffset,
				.IndexOffset = indexOffset,
			};

			Buffer vertices = Buffer((void*)submesh.Vertices.data(), submesh.Vertices.size() * sizeof(Vertex3D));
			Buffer indices = Buffer((void*)submesh.Indices.data(), submesh.Indices.size() * sizeof(uint32_t));

			CreateSubmeshResources(data, vertices, indices);

			m_SubmeshData[data.NodeID] = data;
		}
	}

	void Mesh::GenerateMesh(const std::unordered_map<uint64_t, SubmeshData>& submeshes, Buffer vertexData, Buffer indexData)
	{
		m_SubmeshData = submeshes;

		for (auto& [id, submesh] : m_SubmeshData)
		{
			Buffer vertices = Buffer((uint8_t*)vertexData.Data + submesh.VertexOffset * sizeof(Vertex3D), submesh.VertexCount * sizeof(Vertex3D));
			Buffer indices = Buffer((uint8_t*)indexData.Data + submesh.IndexOffset * sizeof(Vertex3D), submesh.IndexCount * sizeof(uint32_t));

			CreateSubmeshResources(submesh, vertices, indices);
		}
	}

	uint64_t Mesh::CalculateTotalVertexCount()
	{
		HZR_PROFILE_FUNCTION();

		uint64_t count = 0;
		for (auto& [uid, mesh] : m_SubmeshData)
			count += mesh.VertexCount;

		return count;
	}

	uint64_t Mesh::CalculateTotalIndexCount()
	{
		HZR_PROFILE_FUNCTION();

		uint64_t count = 0;
		for (auto& [uid, mesh] : m_SubmeshData)
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

		m_VertexBuffers[handle] = GPUBuffer::Create(&vertexBufferInfo);

		BufferCreateInfo indexBufferInfo = {
			.Name = fmt::format("IndexBuffer {} {}", File::GetName(m_SourceAssetPath), submesh.NodeName),
			.UsageFlags = BUFFER_USAGE_INDEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_SRC,
			.Size = indices.Size,
			.Data = indices.Data,
		};

		m_IndexBuffers[handle] = GPUBuffer::Create(&indexBufferInfo);
	}
}

