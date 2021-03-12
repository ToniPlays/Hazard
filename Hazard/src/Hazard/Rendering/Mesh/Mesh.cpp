#pragma once

#include <hzrpch.h>
#include "Mesh.h"
#include "../RenderCommand.h"
#include "Hazard/Rendering/RenderUtils.h"

namespace Hazard::Rendering {

    Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    {
        this->vertices = vertices;
        this->indices = indices;

        GenerateArrays();
    }
    Mesh::~Mesh()
    {

    }
    void Mesh::Render()
    {
        uint32_t size = meshVAO->GetIndexBuffer()->GetCount();
        RenderCommand::DrawIndexed(meshVAO, size);
    }
    void Mesh::GenerateArrays()
    {
        meshVAO = RenderUtils::Create<VertexArray>();
        VertexBuffer* buffer = RenderUtils::Create<VertexBuffer>((uint32_t)(vertices.size() * sizeof(Vertex)));

        buffer->SetLayout({ 
            { ShaderDataType::Float3, "v_position" },
            { ShaderDataType::Float4, "v_color" },
            { ShaderDataType::Float2, "v-textCoords" }
            });
        buffer->SetData(vertices.data(), vertices.size() * sizeof(Vertex));
        meshVAO->AddBuffer(buffer);

        IndexBuffer* iBuffer = RenderUtils::Create<IndexBuffer>();
        iBuffer->SetData(indices.data(), indices.size());
        meshVAO->SetIndexBuffer(iBuffer);
        
        HZR_CORE_INFO("Vertices {0}, indices {1}", vertices.size(), indices.size());
        shader = RenderUtils::Create<Shader>("res/shaders/PBRShader.glsl");
    }
}