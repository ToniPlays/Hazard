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
    }
    void Mesh::GenerateArrays()
    {
    }
}