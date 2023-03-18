#pragma once

#include "Hazard/Assets/Asset.h"
#include "Hazard/Core/Core.h"
#include "HazardRendererCore.h"
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
		size_t VertexCount;
		Vertex3D* pVertices;
		size_t IndexCount;
		uint32_t* pIndices;
		uint32_t UsageFlags = 0;
	};

	struct SubMesh 
	{
		size_t BaseVertex;
		size_t BaseIndex;
		size_t MaterialIndex;
		size_t IndexCount;
		size_t VertexCount;

		glm::mat4 Transform;
		glm::mat4 LocalTransform;

		std::string NodeName, MeshName;
	};

	class Mesh : public Asset
	{
	public:
		Mesh() = default;
		Mesh(MeshCreateInfo* createInfo);
		Mesh(Ref<HazardRenderer::VertexBuffer> vertexBuffer, Ref<HazardRenderer::IndexBuffer> indexBuffer, Ref<AssetPointer> pipeline);
		~Mesh() = default;

		bool IsValid() { return m_VertexBuffer && m_IndexBuffer; }
		size_t GetIndexCount() { return m_IndexBuffer->GetCount(); }

		Ref<HazardRenderer::VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
		Ref<HazardRenderer::IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
		const BoundingBox& GetBoundingBox() { return m_BoundingBox; }

	private:
		Ref<HazardRenderer::VertexBuffer> m_VertexBuffer = nullptr;
		Ref<HazardRenderer::IndexBuffer> m_IndexBuffer = nullptr;

		Buffer m_LocalVertexData;
		Buffer m_LocalIndexData;

		BoundingBox m_BoundingBox;
	};
}
