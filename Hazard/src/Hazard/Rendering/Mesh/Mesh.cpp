
#include <hzrpch.h>
#include "Mesh.h"
#include "../RenderCommand.h"
#include "MeshFactory.h"
#include "Hazard/Rendering/Pipeline/Pipeline.h"

namespace Hazard::Rendering {

	Mesh::Mesh(const std::string& file, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices) : m_Filename(file), m_Vertices(vertices), m_Indices(indices)
	{
		VertexBufferCreateInfo vertexInfo = {};
		vertexInfo.IsShared = false;
		vertexInfo.DebugName = m_Filename;
		vertexInfo.Size = (uint32_t)m_Vertices.size() * sizeof(Vertex3D);
		vertexInfo.Usage = BufferUsage::StaticDraw;
		vertexInfo.Data = m_Vertices.data();

		m_VertexBuffer = VertexBuffer::Create(&vertexInfo);

		IndexBufferCreateInfo indexInfo = {};
		indexInfo.IsShared = false;
		indexInfo.DebugName = m_Filename;
		indexInfo.Size = (uint32_t)m_Indices.size();
		indexInfo.Data = m_Indices.data();
		indexInfo.Usage = BufferUsage::StaticDraw;

		m_IndexBuffer = IndexBuffer::Create(&indexInfo);
		
	}
	Mesh::Mesh(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline)
	{
		m_VertexBuffer = vertexBuffer;
		m_IndexBuffer = indexBuffer;
		m_Pipeline = pipeline;
	}
	Mesh::~Mesh()
	{

	}
	void Mesh::SetRenderPass(const Ref<RenderPass>& renderPass)
	{
		if (!m_Pipeline) {

			PipelineSpecification spec = {};
			spec.DebugName = "MeshPipeline";
			spec.Usage = PipelineUsage::GraphicsBit;
			spec.DrawType = DrawType::Fill;
			spec.LineWidth = 1.0f;
			spec.ShaderPath = "Shaders/pbr.glsl";
			spec.TargetRenderPass = renderPass;
			spec.IsShared = false;

			m_Pipeline = Pipeline::Create(&spec);
			return;
		}
		if (m_Pipeline->GetSpecifications().TargetRenderPass == renderPass)
			return;
		m_Pipeline->SetRenderPass(renderPass);
	}
}
