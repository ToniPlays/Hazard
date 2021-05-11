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
        m_MeshVAO = RenderUtils::CreateRaw<VertexArray>();
        HZR_CORE_ASSERT(m_MeshVAO, "Mesh VAO failed");
        VertexBuffer* buffer = RenderUtils::CreateRaw<VertexBuffer>((uint32_t)(m_Vertices.size() * sizeof(Vertex)));

        buffer->SetLayout({ 
            { ShaderDataType::Float3, "v_position" },
            { ShaderDataType::Float4, "v_color" },
            { ShaderDataType::Float3, "v_normals" },
            { ShaderDataType::Float2, "v-textCoords" }
            });

        buffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
        m_MeshVAO->AddBuffer(buffer);

        IndexBuffer* iBuffer = RenderUtils::CreateRaw<IndexBuffer>();
        iBuffer->SetData(m_Indices.data(), m_Indices.size());
        m_MeshVAO->SetIndexBuffer(iBuffer);
    }
}