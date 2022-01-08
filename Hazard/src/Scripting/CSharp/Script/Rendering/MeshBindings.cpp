
#include "hzrpch.h"
#include "MeshBindings.h"
#include "Scripting/CSharp/Mono/Mono.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"
#include "Hazard/Assets/AssetManager.h"

#include "mono/jit/jit.h"

using namespace Hazard::Rendering;

namespace Hazard::Scripting::CSharp::Bindings {

    BindMap MeshBindings::GetBindings()
    {
        BindMap result;
        result.reserve(1);

        result.emplace_back("Hazard.Rendering.Mesh::Mesh_Create_Native", (void*)Mesh_Create_Native);
        result.emplace_back("Hazard.Rendering.Mesh::Mesh_Destroy_Native", (void*)Mesh_Destroy_Native);

        return result;
    }
    uint64_t MeshBindings::Mesh_Create_Native(void* createInfo)
    {
        using namespace Hazard::Rendering;
        struct CSharpMesh {
            uint64_t MeshVertexHandle;
            uint64_t MeshIndexHandle;
        };

        CSharpMesh* csMesh = (CSharpMesh*)createInfo;

        MeshCreateInfo meshInfo = {};
        meshInfo.VertexBuffer = AssetManager::GetRuntimeResource<VertexBuffer>(csMesh->MeshVertexHandle);
        meshInfo.IndexBuffer = AssetManager::GetRuntimeResource<IndexBuffer>(csMesh->MeshIndexHandle);

        Ref<Mesh> mesh = MeshFactory::LoadMesh(&meshInfo);

        mesh->IncRefCount();
        return mesh->GetHandle();
    }
    void MeshBindings::Mesh_Destroy_Native(uint64_t assetID)
    {
        AssetManager::GetRuntimeResource<Mesh>(assetID)->DecRefCount();
    }
}
