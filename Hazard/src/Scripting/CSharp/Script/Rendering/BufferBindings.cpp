
#include "hzrpch.h"
#include "BufferBindings.h"
#include "Hazard/Events/Input.h"

#include "Hazard/Rendering/Pipeline/Buffers.h"
#include "Scripting/CSharp/Mono/Mono.h"

#include "mono/jit/jit.h"

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
        using namespace Hazard::Rendering;
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
        
        return 0;
    }

    void BufferBindings::VertexBuffer_Destroy_Native(uint32_t resourceID)
    {

    }

    uint32_t BufferBindings::VertexBuffer_GetSize_Native(uint32_t resourceID)
    {
        using namespace Hazard::Rendering;
        return 0;
    }
    uint64_t BufferBindings::IndexBuffer_Create_Native(void* createInfo)
    {
        using namespace Hazard::Rendering;
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
        
        return 0;
    }
    void BufferBindings::IndexBuffer_Destroy_Native(uint32_t resourceID)
    {

    }
    uint32_t BufferBindings::IndexBuffer_GetSize_Native(uint32_t resourceID)
    {
        return uint32_t();
    }
}
