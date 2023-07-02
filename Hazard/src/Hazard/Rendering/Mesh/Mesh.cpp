#include <hzrpch.h>

#include "Mesh.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "HazardRendererCore.h"
#include "MeshCreateInfo.h"

namespace Hazard
{
	using namespace HazardRenderer;

	Mesh::Mesh(MeshCreateInfo* createInfo)
	{
		HZR_PROFILE_FUNCTION();

		BufferLayout layout = Vertex3D::Layout();

		BufferCreateInfo vboInfo = {};
		vboInfo.Name = createInfo->DebugName;
		vboInfo.Size = createInfo->VertexCount;
		vboInfo.Data = createInfo->pVertices;
		vboInfo.UsageFlags = createInfo->UsageFlags;

		//m_VertexBuffer = GPUBuffer::Create(&vboInfo);

		BufferCreateInfo iboInfo = {};
		iboInfo.Name = createInfo->DebugName;
		iboInfo.Size = createInfo->IndexCount;
		iboInfo.Data = createInfo->pIndices;
		iboInfo.UsageFlags = createInfo->UsageFlags;

		//m_IndexBuffer = GPUBuffer::Create(&iboInfo);
		m_BoundingBox = createInfo->BoundingBox;
	}

	Mesh::Mesh(Ref<HazardRenderer::GPUBuffer> vertexBuffer, Ref<HazardRenderer::GPUBuffer> indexBuffer, Ref<AssetPointer> pipeline) : m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer)
	{
		HZR_PROFILE_FUNCTION();
	}
}

