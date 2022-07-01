#include "MeshFactory.h"

namespace HazardRenderer 
{
    void MeshFactory::SetOptimization(uint32_t flags)
    {
        m_MeshFlags = flags;
    }
    void MeshFactory::LoadMesh(const std::string& file)
    {
        std::cout << "Loading mesh from " << file << std::endl;
    }
}