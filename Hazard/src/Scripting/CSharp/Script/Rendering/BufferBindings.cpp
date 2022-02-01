
#include "hzrpch.h"
#include "BufferBindings.h"
#include "Hazard/Events/Input.h"

#include "Hazard/RenderContext/Pipeline/Buffers.h"
#include "Scripting/CSharp/Mono/Mono.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Rendering/Mesh/Mesh.h"

#include "mono/jit/jit.h"

using namespace Hazard::Rendering;

namespace Hazard::Scripting::CSharp::Bindings {

    BindMap BufferBindings::GetBindings()
    {
        BindMap result;
        result.reserve(6);
        result.emplace_back("Hazard.Rendering.VertexBuffer::VertexBuffer_Create_Native", (void*) VertexBuffer_Create_Native);
        result.emplace_back("Hazard.Rendering.VertexBuffer::VertexBuffer_Destroy_Native", (void*) VertexBuffer_Destroy_Native);
        result.emplace_back("Hazard.Rendering.VertexBuffer::VertexBuffer_GetSize_Native", (void*) VertexBuffer_GetSize_Native);

        result.emplace_back("Hazard.Rendering.IndexBuffer::IndexBuffer_Create_Native", (void*)IndexBuffer_Create_Native);
        result.emplace_back("Hazard.Rendering.IndexBuffer::IndexBuffer_Destroy_Native", (void*)IndexBuffer_Destroy_Native);
        result.emplace_back("Hazard.Rendering.IndexBuffer::IndexBuffer_GetSize_Native", (void*)IndexBuffer_GetSize_Native);
        return result;
    }

    uint64_t BufferBindings::VertexBuffer_Create_Native(void* createInfo)
    {
        struct CSharpInfo {
            uint32_t size;
            BufferUsage usage;
            MonoArray* data;
        };

        CSharpInfo* cInfo = static_cast<CSharpInfo*>(createInfo);

        VertexBufferCreateInfo info = {};
        info.DebugName = "C#CreatedVertexBuffer";
        info.IsShared = false;
        info.Size = cInfo->size;
        info.Usage = cInfo->usage;
        info.Data = cInfo->data ? Mono::GetArrayValuePointer(cInfo->data) : nullptr;

        
        Ref<VertexBuffer> buffer = VertexBuffer::Create(&info);
        buffer->IncRefCount();
        return buffer->GetHandle();
    }

    void BufferBindings::VertexBuffer_Destroy_Native(uint64_t resourceID)
    {
        Ref<VertexBuffer> buffer = AssetManager::GetRuntimeResource<VertexBuffer>(resourceID);
        buffer->DecRefCount();
    }

    uint32_t BufferBindings::VertexBuffer_GetSize_Native(uint64_t resourceID)
    {
        Ref<VertexBuffer> buffer = AssetManager::GetRuntimeResource<VertexBuffer>(resourceID);
        return buffer->GetSize();
    }
    uint64_t BufferBindings::IndexBuffer_Create_Native(void* createInfo)
    {
        
        struct CSharpInfo {
            uint32_t size;
            BufferUsage usage;
            MonoArray* data;
        };

        CSharpInfo* cInfo = (CSharpInfo*)createInfo;
        IndexBufferCreateInfo info = {};
        info.DebugName = "C#CreatedIndexBuffer";
        info.IsShared = false;
        info.Size = cInfo->size;
        info.Usage = cInfo->usage;
        info.Data = cInfo->data ? (uint32_t*)Mono::GetArrayValuePointer(cInfo->data) : nullptr;

        Ref<IndexBuffer> buffer = IndexBuffer::Create(&info);
        buffer->IncRefCount();

        return buffer->GetHandle();
    }
    void BufferBindings::IndexBuffer_Destroy_Native(uint64_t resourceID)
    {
        Ref<IndexBuffer> buffer = AssetManager::GetRuntimeResource<IndexBuffer>(resourceID);
        buffer->DecRefCount();
    }
    uint32_t BufferBindings::IndexBuffer_GetSize_Native(uint64_t resourceID)
    {
        return AssetManager::GetRuntimeResource<IndexBuffer>(resourceID)->GetCount();
    }
}
