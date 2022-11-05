#include "Mesh.h"

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

		VertexBufferCreateInfo vboInfo = {};
		vboInfo.DebugName = "SomeMeshVertexBuffer";
		vboInfo.Layout = &layout;
		vboInfo.Size = createInfo->VertexCount;
		vboInfo.Data = (void*)createInfo->pVertices;
		vboInfo.Usage = createInfo->Usage;

		m_VertexBuffer = VertexBuffer::Create(&vboInfo);

		IndexBufferCreateInfo iboInfo = {};
		iboInfo.DebugName = "SomeMeshIndices";
		iboInfo.Size = createInfo->IndexCount;
		iboInfo.Data = createInfo->pIndices;
		iboInfo.Usage = createInfo->Usage;

		m_IndexBuffer = IndexBuffer::Create(&iboInfo);

		m_Pipeline = ShaderLibrary::GetPipelinePtr("pbr_static");
		m_BoundingBox = createInfo->BoundingBox;
	}

	Mesh::Mesh(Ref<HazardRenderer::VertexBuffer> vertexBuffer, Ref<HazardRenderer::IndexBuffer> indexBuffer, Ref<AssetPointer> pipeline) : m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), m_Pipeline(pipeline)
	{
		HZR_PROFILE_FUNCTION();
		if (pipeline->Value) return;

		m_Pipeline = ShaderLibrary::GetPipelinePtr("pbr_static");
	}
}

