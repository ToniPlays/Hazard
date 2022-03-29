#pragma once

#include "Asset.h"
#include "Backend/Core/Pipeline/Buffers.h"

namespace HazardRenderer 
{
	class RawMesh : public HazardUtility::Asset
	{
	public:
		RawMesh() = default;
		RawMesh(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer) : m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer) {};

		Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; };
		Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; };

	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}