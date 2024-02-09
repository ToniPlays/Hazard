#pragma once

#include "Hazard/Assets/Asset.h"
#include "Hazard/Core/Core.h"
#include "HazardRendererCore.h"
#include "Hazard/Assets/AssetManager.h"
#include "Material.h"
#include "../Vertices.h"
#include <glm/glm.hpp>

#include "BoundingBox.h"

namespace Hazard
{
	struct MeshData;

	struct MeshCreateInfo
	{
        std::string DebugName;
		BoundingBox BoundingBox;
		uint64_t VertexCount;
		Vertex3D* pVertices;
		uint64_t IndexCount;
		uint32_t* pIndices;
	};

	struct SubMesh 
	{
		uint64_t BaseVertex;
		uint64_t BaseIndex;
		uint64_t MaterialIndex;
		uint64_t IndexCount;
		uint64_t VertexCount;

		glm::mat4 Transform;
		glm::mat4 LocalTransform;

		std::string NodeName, MeshName;
	};

	class Mesh : public Asset
	{
	public:
		Mesh() = default;
		Mesh(MeshCreateInfo* createInfo);
		Mesh(Ref<HazardRenderer::GPUBuffer> vertexBuffer, Ref<HazardRenderer::GPUBuffer> indexBuffer, Ref<AssetPointer> pipeline);
		~Mesh() = default;

		AssetType GetType() const override { return AssetType::Mesh; }

		bool IsValid() { return m_VertexBuffer && m_IndexBuffer; }
		uint64_t GetIndexCount() { return m_IndexBuffer->GetSize() / 3; }

		Ref<HazardRenderer::GPUBuffer> GetVertexBuffer() { return m_VertexBuffer; }
		Ref<HazardRenderer::GPUBuffer> GetIndexBuffer() { return m_IndexBuffer; }

		Ref<Material> GetMaterial() { return AssetManager::GetAsset<Material>(GetMaterialHandle()); }
		AssetHandle GetMaterialHandle() { return m_MaterialHandle; }
		const BoundingBox& GetBoundingBox() { return m_BoundingBox; }

	private:
		Ref<HazardRenderer::GPUBuffer> m_VertexBuffer = nullptr;
		Ref<HazardRenderer::GPUBuffer> m_IndexBuffer = nullptr;
		AssetHandle m_MaterialHandle = INVALID_ASSET_HANDLE;

		Buffer m_LocalVertexData;
		Buffer m_LocalIndexData;

		BoundingBox m_BoundingBox;
	};
}
