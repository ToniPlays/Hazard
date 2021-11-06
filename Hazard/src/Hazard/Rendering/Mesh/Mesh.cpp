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
        vertexInfo.Data = m_Vertices.data();
        vertexInfo.Usage = BufferUsage::StaticDraw;

        IndexBufferCreateInfo indexInfo = {};
        indexInfo.Size = m_Indices.size();
        indexInfo.Data = m_Indices.data();
        indexInfo.Usage = BufferUsage::StaticDraw;

        PipelineSpecification spec = {};
        spec.Usage = PipelineUsage::GraphicsBit;
        spec.pVertexBuffer = &vertexInfo;
        spec.pIndexBuffer = &indexInfo;
        spec.ShaderPath = "res/Shaders/sources/pbr.glsl";

        m_Pipeline = Pipeline::Create(spec);
    }
}