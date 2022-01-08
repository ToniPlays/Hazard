
#include "hzrpch.h"
#include "MeshBindings.h"
#include "Scripting/CSharp/Mono/Mono.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"

#include "mono/jit/jit.h"

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
            uint64_t MeshVertexBuffer;
            uint64_t MeshIndexBuffer;
        };

        CSharpMesh* csMesh = (CSharpMesh*)createInfo;

        MeshCreateInfo meshInfo = {};
        //meshInfo.VertexBuffer = ScriptResourceManager::GetResource<VertexBuffer>(csMesh->MeshVertexBuffer);
        //meshInfo.IndexBuffer = ScriptResourceManager::GetResource<IndexBuffer>(csMesh->MeshIndexBuffer);

        Ref<Mesh> mesh =  Ref<Mesh>::Create(meshInfo.VertexBuffer, meshInfo.IndexBuffer, nullptr);
        mesh->IncRefCount();
        return mesh->GetHandle();
    }
    void MeshBindings::Mesh_Destroy_Native(uint64_t assetID)
    {
        
    }
}
