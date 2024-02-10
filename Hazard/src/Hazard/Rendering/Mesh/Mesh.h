#pragma once

#include "Hazard/Assets/Asset.h"
#include "Hazard/Core/Core.h"
#include "HazardRendererCore.h"
#include "Hazard/Assets/AssetManager.h"
#include "Importers/MeshImporter.h"
#include "../Vertices.h"

namespace Hazard
{
	class Mesh : public Asset
	{
	public:
		Mesh() = default;
		~Mesh() = default;

		AssetType GetType() const override { return AssetType::Mesh; }
		void GenerateMesh(const std::vector<MeshImporter::MeshData>& meshData);

		Ref<HazardRenderer::GPUBuffer> GetVertexBuffer() { return m_VertexBuffer; };
		Ref<HazardRenderer::GPUBuffer> GetIndexBuffer() { return m_IndexBuffer; }

	private:
		uint64_t CalculateTotalVertexCount(const std::vector<MeshImporter::MeshData>& meshData);
		uint64_t CalculateTotalIndexCount(const std::vector<MeshImporter::MeshData>& meshData);

	private:
		Ref<HazardRenderer::GPUBuffer> m_VertexBuffer;
		Ref<HazardRenderer::GPUBuffer> m_IndexBuffer;

	};
}
