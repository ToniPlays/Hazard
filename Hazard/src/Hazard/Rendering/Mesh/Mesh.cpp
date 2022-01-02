#pragma once

#include <hzrpch.h>
#include "Mesh.h"
#include "../RenderCommand.h"
#include "MeshFactory.h"
#include "Hazard/Rendering/Pipeline/Pipeline.h"

namespace Hazard::Rendering {

	Mesh::Mesh(const std::string& file, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices) : m_Filename(file), m_Vertices(vertices), m_Indices(indices)
	{
		GeneratePipeline();
	}
	Mesh::~Mesh()
	{

	}
	void Mesh::GeneratePipeline()
	{
		VertexBufferCreateInfo vertexInfo = {};
		vertexInfo.Size = m_Vertices.size() * sizeof(Vertex3D);
		vertexInfo.Usage = BufferUsage::StaticDraw;
		vertexInfo.Data = m_Vertices.data();

		m_VertexBuffer = VertexBuffer::Create(&vertexInfo);

		IndexBufferCreateInfo indexInfo = {};
		indexInfo.Size = m_Indices.size();
		indexInfo.Data = m_Indices.data();
		indexInfo.Usage = BufferUsage::StaticDraw;

		m_IndexBuffer = IndexBuffer::Create(&indexInfo);
	}
	void Mesh::SetRenderPass(const Ref<RenderPass>& renderPass)
	{
		if (!m_Pipeline) {

			PipelineSpecification spec = {};
			spec.Usage = PipelineUsage::GraphicsBit;
			spec.DrawType = DrawType::Fill;
			spec.LineWidth = 1.0f;
			spec.ShaderPath = "Shaders/pbr.glsl";
			spec.TargetRenderPass = renderPass;

			m_Pipeline = Pipeline::Create(&spec);
			return;
		}
		if (m_Pipeline->GetSpecifications().TargetRenderPass == renderPass)
			return;

		auto spec = m_Pipeline->GetSpecifications();
		spec.TargetRenderPass = renderPass;
		m_Pipeline = Pipeline::Create(&spec);
	}
}
