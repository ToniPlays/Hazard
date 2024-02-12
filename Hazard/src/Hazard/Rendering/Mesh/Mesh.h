#pragma once

#include "Hazard/Assets/Asset.h"
#include "Hazard/Core/Core.h"
#include "HazardRendererCore.h"
#include "Hazard/Assets/AssetManager.h"
#include "Importers/MeshImporter.h"
#include "../Vertices.h"

namespace Hazard
{
	struct SubmeshData
	{
		std::string NodeName;
		uint64_t NodeID;
		uint32_t VertexCount;
		uint32_t IndexCount;
		uint32_t VertexOffset;
		uint32_t IndexOffset;
	};

	class Mesh : public Asset
	{
	public:
		Mesh() = default;
		~Mesh() = default;

		AssetType GetType() const override { return AssetType::Mesh; }
		void GenerateMesh(const std::vector<MeshImporter::MeshData>& meshData);
		void GenerateMesh(const std::unordered_map<uint64_t, SubmeshData>& submeshes, Buffer vertexData, Buffer indexData);

		const std::unordered_map<uint64_t, SubmeshData> GetSubmeshData() const { return m_SubmeshData; }

		Ref<HazardRenderer::GPUBuffer> GetVertexBuffer(uint64_t submeshId) const { return m_VertexBuffers.at(submeshId); };
		Ref<HazardRenderer::GPUBuffer> GetIndexBuffer(uint64_t submeshId) const { return m_IndexBuffers.at(submeshId); }

		uint64_t CalculateTotalVertexCount();
		uint64_t CalculateTotalIndexCount();

	private:
		void CreateSubmeshResources(const SubmeshData& submesh, Buffer vertices, Buffer indices);

	private:
		std::unordered_map<uint64_t, Ref<HazardRenderer::GPUBuffer>> m_VertexBuffers;
		std::unordered_map<uint64_t, Ref<HazardRenderer::GPUBuffer>> m_IndexBuffers;
		std::unordered_map<uint64_t, SubmeshData> m_SubmeshData;

	};
}
