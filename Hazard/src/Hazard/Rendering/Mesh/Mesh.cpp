#pragma once

#include <hzrpch.h>
#include "Mesh.h"
#include "../RenderCommand.h"
#include "Hazard/Rendering/RenderUtils.h"

namespace Hazard::Rendering {

    Mesh::Mesh(std::string file, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    {
        this->m_Filename = file;
        this->m_Vertices = vertices;
        this->m_Indices = indices;

        GenerateArrays();
    }
    Mesh::~Mesh()
    {

    }
    void Mesh::Render()
    {
        RenderCommand::GetStats().vertices += m_Vertices.size();
        uint32_t size = m_MeshVAO->GetIndexBuffer()->GetCount();
        
        RenderCommand::DrawIndexed(m_MeshVAO, size);
    }
    void Mesh::GenerateArrays()
    {
        BufferLayout layout = {
            { ShaderDataType::Float3, "v_position" },
            { ShaderDataType::Float4, "v_color" },
            { ShaderDataType::Float3, "v_normals" },
            { ShaderDataType::Float2, "v-textCoords" }
        };

        VertexBufferCreateInfo bufferInfo;
        bufferInfo.layout = &layout;
        bufferInfo.data = m_Vertices.data();
        bufferInfo.size = m_Vertices.size() * sizeof(Vertex);
        bufferInfo.dataStream = DataStream::StaticDraw;

        IndexBufferCreateInfo indexBufferInfo;
        indexBufferInfo.size = m_Indices.size();
        indexBufferInfo.data = m_Indices.data();

        VertexArrayCreateInfo info;
        info.bufferInfo = &bufferInfo;
        info.indexBufferInfo = &indexBufferInfo;

        m_MeshVAO = RenderUtils::CreateRaw<VertexArray>(info);
        HZR_CORE_ASSERT(m_MeshVAO, "Mesh VAO failed");
    }
}