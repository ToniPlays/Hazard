#include <hzrpch.h>

#include "Mesh.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "HazardRendererCore.h"

namespace Hazard
{
    void Mesh::GenerateMesh(const std::vector<MeshImporter::MeshData>& meshData)
    {
        uint64_t totalVertexCount = CalculateTotalVertexCount(meshData);
        uint64_t totalIndexCount = CalculateTotalIndexCount(meshData);

        BufferCreateInfo vertexBufferInfo = {};
        vertexBufferInfo.Name = "Mesh vertex buffer";
        vertexBufferInfo.Size = totalVertexCount * sizeof(Vertex3D);
        vertexBufferInfo.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT;

        m_VertexBuffer = GPUBuffer::Create(&vertexBufferInfo);
        
        BufferCreateInfo indexBufferInfo = {};
        indexBufferInfo.Name = "Mesh index buffer";
        indexBufferInfo.Size = totalIndexCount * sizeof(uint32_t);
        indexBufferInfo.UsageFlags = BUFFER_USAGE_INDEX_BUFFER_BIT;

        m_IndexBuffer = GPUBuffer::Create(&indexBufferInfo);

        IncRefCount();
        IncRefCount();
        IncRefCount();
        IncRefCount();
        IncRefCount();
    }
    uint64_t Mesh::CalculateTotalVertexCount(const std::vector<MeshImporter::MeshData>& meshData)
    {
        uint64_t count = 0;
        for (auto& mesh : meshData)
            count += mesh.Vertices.size();
        return count;
    }
    uint64_t Mesh::CalculateTotalIndexCount(const std::vector<MeshImporter::MeshData>& meshData)
    {
        uint64_t count = 0;
        for (auto& mesh : meshData)
            count += mesh.Indices.size();
        return count;
    }
}

