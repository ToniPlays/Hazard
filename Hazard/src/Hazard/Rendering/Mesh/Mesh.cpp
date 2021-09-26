#pragma once

#include <hzrpch.h>
#include "Mesh.h"
#include "../RenderCommand.h"
#include "MeshFactory.h"

namespace Hazard::Rendering {

    Mesh::Mesh(const std::string& file, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) : m_Filename(file), m_Vertices(vertices), m_Indices(indices)
    {
        GenerateArrays();
    }
    Mesh::~Mesh()
    {
        delete m_MeshVAO;
    }
    void Mesh::GenerateArrays()
    {
        BufferLayout layout = {
            { "v_position",     ShaderDataType::Float3 },
            { "v_color",        ShaderDataType::Float4 },
            { "v_normals",      ShaderDataType::Float3 },
            { "v_textCoords",   ShaderDataType::Float2 }
        };

        VertexBufferCreateInfo bufferInfo;
        bufferInfo.Layout = &layout;
        bufferInfo.Data = m_Vertices.data();
        bufferInfo.Size = m_Vertices.size() * sizeof(Vertex);
        bufferInfo.Usage = BufferUsage::StaticDraw;

        IndexBufferCreateInfo indexBufferInfo;
        indexBufferInfo.Size = m_Indices.size();
        indexBufferInfo.Data = m_Indices.data();
        indexBufferInfo.Usage = BufferUsage::StaticDraw;

        VertexArrayCreateInfo info;
        info.VertexBuffer = &bufferInfo;
        info.IndexBuffer = &indexBufferInfo;

        m_MeshVAO = VertexArray::Create(info);
        HZR_CORE_ASSERT(m_MeshVAO, "Mesh VAO failed");
    }
}