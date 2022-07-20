#include "Mesh.h"

#include <hzrpch.h>
#include "Mesh.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard
{
	using namespace HazardRenderer;

	Mesh::Mesh(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices)
	{
		BufferLayout layout = { { "v_Position",			ShaderDataType::Float3 },
								{ "v_Color",			ShaderDataType::Float4 },
								{ "v_Normal",			ShaderDataType::Float3 },
								{ "v_TextureCoords",	ShaderDataType::Float2 }
		};

		m_LocalVertexData = Buffer::Copy(vertices.data(), vertices.size() * sizeof(Vertex3D));
		m_LocalIndexData = Buffer::Copy(indices.data(), indices.size() * sizeof(uint32_t));

		VertexBufferCreateInfo vboInfo = {};
		vboInfo.DebugName = "SomeMesh";
		vboInfo.Layout = &layout;
		vboInfo.Size = m_LocalVertexData.Size;
		vboInfo.Data = m_LocalVertexData.Data;
		vboInfo.Usage = BufferUsage::StaticDraw;

		m_VertexBuffer = VertexBuffer::Create(&vboInfo);

		IndexBufferCreateInfo iboInfo = {};
		iboInfo.DebugName = "SomeMeshIndices";
		iboInfo.Size = m_LocalIndexData.Size;
		iboInfo.Data = (uint32_t*)m_LocalIndexData.Data;
		iboInfo.Usage = BufferUsage::StaticDraw;

		m_IndexBuffer = IndexBuffer::Create(&iboInfo);

		PipelineSpecification pipelineSpecs = {};
		pipelineSpecs.DebugName = "PBRShader";
		pipelineSpecs.Usage = PipelineUsage::GraphicsBit;
		pipelineSpecs.DrawType = DrawType::Fill;
		pipelineSpecs.ShaderPath = "Shaders/pbr.glsl";
		pipelineSpecs.pTargetRenderPass = Application::GetModule<RenderEngine>().GetWindow().GetSwapchain()->GetRenderPass().Raw();
		pipelineSpecs.pBufferLayout = &layout;

		m_Pipeline = Pipeline::Create(&pipelineSpecs);
	}

	Mesh::Mesh(Ref<HazardRenderer::VertexBuffer> vertexBuffer, Ref<HazardRenderer::IndexBuffer> indexBuffer, Ref<HazardRenderer::Pipeline> pipeline) : m_VertexBuffer(vertexBuffer), m_IndexBuffer(m_IndexBuffer), m_Pipeline(pipeline)
	{

	}
}

